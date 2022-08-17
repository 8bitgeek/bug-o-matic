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
#include <piezo-tx.h>
#include <board.h>
#include <syslog.h>
#include <mb.h>
#include <mbport.h>
#include <caribou/lib/mutex.h>
#include <xduc-state-machine.h>
#include <xduc-reg-map.h>
#include <led.h>
#include <hw_pinfire.h>
#include <rx-gain.h>
#include <serialno.h>

typedef struct _xduc_t_
{
    uint16_t*       xduc_reg_map;
    uint16_t        xduc_nreg;
    uint16_t        xduc_start_reg;
    jiffies_t       heartbeat_start;
    led_t           led0;
    led_t           led1;
    led_t           led2;
    bool            triggered;
    hw_pinfire_t    pinfire;
    serialno_t      serial_no;
} xduc_t;

static xduc_t xduc;

static uint32_t          	modbus_stack[ MODBUS_THREAD_STK_SZ/sizeof(uint32_t) ];
static caribou_thread_t* 	modbus_thread_id = NULL;

static bool modbus_start            (void);
static void modbus_thread           (void*);
static void xduc_run                (void);
static void update_xduc             (void);
static bool xduc_arm_callback       (void);
static void xduc_armed_callback     (void);
static void xduc_wait_callback      (void);
static void xduc_completed_callback (void);
static void xduc_timeout_callback   (void);
static void feature_setup           (void);
static void update_registers        (void);

extern void feature_main(void* arg)
{
    feature_setup();

    if ( modbus_start() )
    {
        xduc_state_machine_setup( 
                                1, 
                                XDUC_TIMEOUT_MS,
                                xduc_arm_callback,
                                xduc_armed_callback, 
                                xduc_wait_callback, 
                                xduc_completed_callback,
                                xduc_timeout_callback 
                                );

        hw_pinfire_setup( 
                            &xduc.pinfire, 
                            &gpio_rs485_etr,
                            XDUC_TIMEOUT_MS,
                            piezo_tx_trigger)
                        ;

        for( ;; )
        {
            update_xduc();
            xduc_state_machine_service();
        }
    }
}

static bool xduc_arm_callback(void)
{
    uint16_t arm = modbus_slave_register_map_get(XDUC_REG_CTRL) & XDUC_REG_CTRL_ARM;
    if ( arm )
    {
        modbus_slave_register_map_put( XDUC_REG_STATUS, 0 );
        hw_pinfire_arm(&xduc.pinfire);
        return true;
    }
    return false;
}

static void xduc_armed_callback(void)
{
}

static void xduc_wait_callback(void)
{
    // caribou_thread_lock();
    // if ( !xduc.triggered && !hw_gpio_state(&gpio_rs485_etr) )
    // {
    //     xduc.triggered=true;       
    //     hw_gpio_set(&gpio_test0);
    //     piezo_tx_trigger();
    //     hw_gpio_reset(&gpio_test0); 
    // }
    // caribou_thread_unlock();
}

static void xduc_completed_callback(void)
{
    xduc.triggered=false;       
    hw_pinfire_disarm(&xduc.pinfire);
    modbus_slave_register_map_put( XDUC_REG_CTRL, modbus_slave_register_map_get(XDUC_REG_CTRL) & ~XDUC_REG_CTRL_ARM );
    modbus_slave_register_map_put( XDUC_REG_HEARTBEAT, modbus_slave_register_map_get( XDUC_REG_HEARTBEAT )+1 );
    modbus_slave_register_map_put( XDUC_REG_STATUS, XDUC_REG_STATUS_COMPLETED );
}

static void xduc_timeout_callback(void)
{
    xduc.triggered=false;       
    hw_pinfire_disarm(&xduc.pinfire);
    modbus_slave_register_map_put( XDUC_REG_STATUS, XDUC_REG_STATUS_TIMEOUT );
    modbus_slave_register_map_put( XDUC_REG_CTRL, modbus_slave_register_map_get(XDUC_REG_CTRL) & ~XDUC_REG_CTRL_ARM );
}


static void feature_setup(void)
{
    memset(&xduc,0,sizeof(xduc_t));

    led_setup(&xduc.led0,&gpio_led0,LED0_POLARITY);
    led_setup(&xduc.led1,&gpio_led1,LED1_POLARITY);
    led_setup(&xduc.led2,&gpio_led2,LED1_POLARITY);

	rx_gain_setup();

	piezo_tx_setup();

    xduc.xduc_reg_map = xduc_reg_map;
    xduc.xduc_nreg = XDUC_HOLDING_NREGS;
    xduc.xduc_start_reg = XDUC_HOLDING_START;

    xduc.heartbeat_start = jiffies();

    serialno_get(&xduc.serial_no);

    /* flish the input buffer */
    while( fgetc(stdin) >= 0 );

    modbus_slave_register_map_setup(NULL,NULL,NULL,NULL);
    modbus_slave_register_map_set_holding( xduc.xduc_reg_map, xduc.xduc_start_reg, xduc.xduc_nreg );
    modbus_slave_register_map_put( XDUC_REG_VERSION, PRODUCT_VERSON_WORD);
    modbus_slave_register_map_put( XDUC_REG_CTRL,0);
    modbus_slave_register_map_put( XDUC_REG_TX_DURATION,    (uint16_t)piezo_tx_get_driver_cycles());
    modbus_slave_register_map_put( XDUC_REG_TX_GAIN, 	    (uint16_t)piezo_tx_get_output_power());
    modbus_slave_register_map_put( XDUC_REG_RX_GAIN, 	    (uint16_t)rx_gain_get_step());

    modbus_slave_register_map_put( XDUC_REG_AFX_SERIAL_NO_0, xduc.serial_no.hword[0] );
    modbus_slave_register_map_put( XDUC_REG_AFX_SERIAL_NO_1, xduc.serial_no.hword[1] );
    modbus_slave_register_map_put( XDUC_REG_AFX_SERIAL_NO_2, xduc.serial_no.hword[2] );
    modbus_slave_register_map_put( XDUC_REG_AFX_SERIAL_NO_3, xduc.serial_no.hword[3] );
    modbus_slave_register_map_put( XDUC_REG_AFX_SERIAL_NO_4, xduc.serial_no.hword[4] );
    modbus_slave_register_map_put( XDUC_REG_AFX_SERIAL_NO_5, xduc.serial_no.hword[5] );
}

static void update_xduc(void)
{
    if ( xduc_state_machine_is_idle() )
    {
        rx_gain_set_step(           modbus_slave_register_map_get( XDUC_REG_RX_GAIN     ) );
        piezo_tx_set_output_power(  modbus_slave_register_map_get( XDUC_REG_TX_GAIN     ) );
        piezo_tx_set_driver_cycles( modbus_slave_register_map_get( XDUC_REG_TX_DURATION ) );
    }
}

void assert(int cond)
{
    while(!cond);
}

static bool modbus_start( void )
{
    modbus_thread_id = caribou_thread_create(
                    "modbus", 
                    modbus_thread, 
                    NULL, 
                    NULL,
                    modbus_stack,
                    MODBUS_THREAD_STK_SZ, 
                    1,
                    0
                );
    return modbus_thread_id != NULL;
}

static void modbus_thread( void* arg )
{
    for(;;)
    {
        if(eMBInit( MB_RTU, MODBUS_SLAVE_ID, RS485_UART, XDUC_BAUD_RATE, MB_PAR_NONE )==MB_ENOERR)
        {
            eMBEnable();
            for(;;)
            {       
                if( modbus_slave_register_map_service() != MB_ENOERR )
                    fprintf( stderr, "eMBPoll error\n" );
                caribou_thread_yield();
            }
        }
        else
        {
            fprintf( stderr, "eMBInit error\n" );
        }
    }
}

