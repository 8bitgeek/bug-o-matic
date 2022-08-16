/*****************************************************************************
*          ___  _     _ _                  _                 _               *
*         / _ \| |   (_) |                | |               | |              *
*        | (_) | |__  _| |_ __ _  ___  ___| | __  _ __   ___| |_             *
*         > _ <| '_ \| | __/ _` |/ _ \/ _ \ |/ / | '_ \ / _ \ __|            *
*        | (_) | |_) | | || (_| |  __/  __/   < _| | | |  __/ |_             *
*         \___/|_.__/|_|\__\__, |\___|\___|_|\_(_)_| |_|\___|\__|            *
*                           __/ |                                            *
*                          |___/                                             *                                                                                       
*                                                                            *
*     Copyright © 2021 8bitgeek.net                                          *
*     Author: Mike Sharkey <mike@8bitgeek.net>                               *
*                                                                            *
*****************************************************************************/
#include <board.h>
#include <delay.h>
#include <board.h>
#include <mb.h>
#include <mbport.h>
#include <caribou/lib/mutex.h>
#include <modbus-slave-register-map.h>
#include <led.h>
#include <filesystem.h>
#include "registers.h"
#include "tof-run.h"
#include "settings.h"
#include "units.h"
#include "serialno.h"
#include <hw_gpio.h>
#include <bootstrap.h>
#include <button.h>
#include <math.h>
#include <xprintf.h>

#define DEFAULT_DETECT_AMPLITUDE    (15)                        /* uint16_t Detection Amplitude (0-100) */
#define MAX_VELOCITY_BUFFER         (100)                       /* Maximum size of velocity low-pass buffer */
#define FIRMWARE_VERSION            (0x0001)                    /* Firmware Version X.Y */
#define SOS_METERS_SEC              (343.0)                     /* speed of sound in meters/sec */
#define SOS_METERS_USEC             SOS_METERS_SEC/1000000.0    /* speed of sound in meters/usec */
#define t2meters(t)                 SOS_METERS_USEC*(t)         /* t (microseconds) to meters */       
#define t2millimeters(t)            t2meters((t))*1000.0        /* t (microseconds) to millimeters */       
#define MODBUS_THREAD_STK_SZ        (1024*2)

#define to_radians(degrees) ((3.14159265359 * (degrees))/180.0)
#define angle_of(hypotenuse,adjacent) asin(hypotenuse/adjacent)

typedef struct 
{
    uint32_t            modbus_stack[ MODBUS_THREAD_STK_SZ/sizeof(uint32_t) ];
    settings_t          settings;
    float               velocity_buffer[MAX_VELOCITY_BUFFER];
    average_t           velocity_low_pass;
    serialno_t          serial_no;
    caribou_thread_t*   modbus_thread;
    button_t            button;
    led_t               led;
    jiffies_t           last_tick;
    uint16_t            fault_count;
    uint16_t            null_reading_clear_start;
} aq_state_t;

static aq_state_t aq;

static void     modbus_thread          ( void* arg );
static void     modbus_update          ( void );
static void     modbus_update_times    ( void );
static uint16_t modbus_write_notify  ( uint16_t addr, uint16_t val );
static void     filesystem_callback    ( void );
static void     button_callback        ( button_event_t button_event );
static void     bootstrap_callback     ( boot_state_t state ); 

extern "C" void feature_main(void* arg)
{
    caribou_gpio_reset( &gpio_rs485_dir );
    led_setup(&aq.led,&gpio_led0,LED0_POLARITY);
    filesystem_setup(filesystem_callback);
    settings_setup(&aq.settings,&aq.led);
    average_setup(&aq.velocity_low_pass,aq.velocity_buffer,0);
    tof_setup();
    serialno_get(&aq.serial_no);
    bootstrap_setup(bootstrap_callback);
    button_setup(&aq.button,&gpio_btn0,true,button_callback);
    aq.last_tick = jiffies();

    modbus_slave_register_map_setup(NULL,modbus_write_notify,NULL,NULL);
    modbus_slave_register_map_set_holding( settings_get_holding(&aq.settings), REG_HOLDING_START, settings_get_nholding(&aq.settings) );
    modbus_slave_register_map_put           ( HOLD_REG_FW_VERSION,         FIRMWARE_VERSION );
    modbus_slave_register_map_put           ( HOLD_REG_RX_GAIN,            DEFAULT_XDUC_RX_GAIN );
    modbus_slave_register_map_put           ( HOLD_REG_TX_GAIN,            DEFAULT_XDUC_TX_POWER );
    modbus_slave_register_map_put           ( HOLD_REG_DURATION,           DEFAULT_XDUC_TX_DURATION );
    modbus_slave_register_map_put           ( HOLD_REG_DETECT_AMPLITUDE,   DEFAULT_DETECT_AMPLITUDE );
    modbus_slave_register_map_put           ( HOLD_REG_BAUD_RATE_100,      settings_get_baud(&aq.settings)/100 );
    modbus_slave_register_map_put           ( HOLD_REG_SLAVE_ID,           settings_get_id(&aq.settings) );
    modbus_slave_register_map_put_float32   ( HOLD_REG_CORRECTION_FACTOR,  1.0f );
    modbus_slave_register_map_put           ( HOLD_REG_XFR_BYTES_PSIZE,    DEFAULT_XFER_PAGE_SIZE );

    modbus_slave_register_map_put           ( HOLD_REG_AQ_SERIAL_NO_0,     aq.serial_no.hword[0] );
    modbus_slave_register_map_put           ( HOLD_REG_AQ_SERIAL_NO_1,     aq.serial_no.hword[1] );
    modbus_slave_register_map_put           ( HOLD_REG_AQ_SERIAL_NO_2,     aq.serial_no.hword[2] );
    modbus_slave_register_map_put           ( HOLD_REG_AQ_SERIAL_NO_3,     aq.serial_no.hword[3] );
    modbus_slave_register_map_put           ( HOLD_REG_AQ_SERIAL_NO_4,     aq.serial_no.hword[4] );
    modbus_slave_register_map_put           ( HOLD_REG_AQ_SERIAL_NO_5,     aq.serial_no.hword[5] );

    aq.modbus_thread = caribou_thread_create( "modbus", modbus_thread, NULL, NULL,
                                                aq.modbus_stack, MODBUS_THREAD_STK_SZ, 2, 0);
                                                
    for(;;)
    {
        filesystem_service();
        settings_service(&aq.settings);
        modbus_update();
        tof_run();
        led_service(&aq.led);
        bootstrap_service();
        button_service(&aq.button);
    }
}

static void button_callback( button_event_t button_event )
{
    if ( bootstrap_state() == boot_armed && button_event == button_up )
    {
        xfprintf( xstderr, "bootstrap_boot()\n" );
        bootstrap_boot();
    }
}

/**
 * @brief Callback on MicroSD card insertion/mounting state change.
 * Use @ref filesystem_is_media() and @ref filesystem_is_mounted() to
 * determine the current state.
 */
static void filesystem_callback( void )
{
    if ( /* filesystem_is_media() && */ filesystem_is_mounted() )
    {
        modbus_slave_register_map_put( HOLD_REG_BAUD_RATE_100, settings_get_baud(&aq.settings)/100 );
        modbus_slave_register_map_put( HOLD_REG_SLAVE_ID,      settings_get_id(&aq.settings) );

        if ( button_pressed(&aq.button) )
        {
            xfprintf( xstderr, "bootstrap_arm()\n" );
            led_reset(&aq.led);
            led_blink(&aq.led,LED_BLINK_FOREVER,BLINK_RATE_HI);
            bootstrap_arm();
        }
    }
}

static void bootstrap_callback( boot_state_t state )
{
}

static void modbus_thread( void* arg )
{
    for( ;; )
    {
        uint32_t baud =  modbus_slave_register_map_get( HOLD_REG_BAUD_RATE_100 )*100;
        uint8_t  id   =  modbus_slave_register_map_get( HOLD_REG_SLAVE_ID );

        if ( id != 255 && id != 0 )
        {
            xfprintf( xstderr, "%d %d\n", baud, id );

            if(eMBInit( MB_RTU, id, RS485_UART, baud, MB_PAR_NONE )==MB_ENOERR)
            {
                eMBEnable();
                for(;;)
                {
                    if( modbus_slave_register_map_service() != MB_ENOERR )
                        xfprintf( xstderr, "eMBPoll error\n");
                    caribou_thread_yield();
                }
            }
        }
        caribou_thread_yield();
    }
}

/**
 * Handle cases where we need to take action or contrain the written value 
 */
static uint16_t modbus_write_notify(uint16_t addr, uint16_t val)
{
    if ( modbus_slave_register_map_get( addr ) != val )
    {
        switch ( addr )
        {
            case HOLD_REG_BAUD_RATE_100:
                if ( settings_get_baud(&aq.settings) != val*100 )
                    settings_set_baud(&aq.settings,val*100);   
                break;
            case HOLD_REG_SLAVE_ID:
                if ( settings_get_id(&aq.settings) != val )
                    settings_set_id(&aq.settings,val);     
                break;
            case HOLD_REG_LOW_PASS_FILTER:
                val = val > MAX_VELOCITY_BUFFER ? MAX_VELOCITY_BUFFER : val;
                average_set_size(&aq.velocity_low_pass,val);
                break;
            case HOLD_REG_DETECT_AMPLITUDE:
                val = val >= 100 ? 100 : val;
                // tof_detect_setup(val);
                break;
        }
        settings_set_dirty(&aq.settings,true);
    }
    return val;
}

static void modbus_update(void)
{
    modbus_update_times();
}

static void modbus_update_times( void )
{
    /* Tick the seconds-counter register */
    if ( jiffies() - aq.last_tick >= 1000 )
    {
        uint16_t seconds = modbus_slave_register_map_get( HOLD_REG_SECONDS);
        uint16_t minutes = modbus_slave_register_map_get( HOLD_REG_MINUTES);
        uint16_t hours = modbus_slave_register_map_get( HOLD_REG_HOURS);
        uint16_t days = modbus_slave_register_map_get( HOLD_REG_DAYS);

        aq.last_tick = jiffies();
        modbus_slave_register_map_put( HOLD_REG_COUNTER,modbus_slave_register_map_get( HOLD_REG_COUNTER)+1);

        /* ripple count the uptime counters */
        caribou_thread_lock();
            if ( ++seconds >= 60 )
            {
                seconds = 0;
                if ( ++minutes >= 60 )
                {
                    minutes=0;
                    if ( ++hours >= 24 )
                    {
                        modbus_slave_register_map_put( HOLD_REG_DAYS, ++days );
                    }
                    modbus_slave_register_map_put( HOLD_REG_HOURS, hours );
                }
                modbus_slave_register_map_put( HOLD_REG_MINUTES, minutes );
            }
            modbus_slave_register_map_put( HOLD_REG_SECONDS, seconds );
        caribou_thread_unlock();

    }
}

