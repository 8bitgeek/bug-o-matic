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
#include "tof-run.h"
#include <hw_usart.h>
#include <modbus.h>
#include <delay.h>
#include <led.h>
#include <xduc-reg-map.h>
#include "registers.h"
#include <modbus-slave-register-map.h>
#include <modbus-disco-master.h>
#include <hw_pintrigger.h>
#include <hw_xduc_sram_dma.h>
#include <xprintf.h>

#define MAX_DISCO_TIMEOUT   (15000)

typedef enum 
{
    state_disco_start=0,
    state_disco_wait,
    state_idle,
    state_delay_start,
    state_delay_wait,
    state_set_gains,
    state_tx_arm,
    state_tx_wait,
    state_tx_trigger,
    state_rx_wait,
    state_rx,
    state_get_vars,
    state_log,
    state_change_dir,
} tof_state_t;

typedef struct _tof_t_
{
    led_t*                  status_led;
    tof_state_t             state;
    jiffies_t               time_start;
    int                     rc;

    uint16_t*               xduc_reg_map;
    uint16_t                xduc_nreg;
    uint16_t                xduc_start_reg;

    hw_pintrigger_t         pintrigger;
    modbus_t*               modbus_ctx;
    modbus_disco_list_t*    list;
    int                     list_index;

    uint16_t                xduc_rx_gain;
    uint16_t                xduc_tx_duration;
    float                   xduc_tx_power;

    bool                    disco_complete;
} tof_t;

static tof_t tof;

static void rx_capture_callback	        (uint16_t* capture,size_t nsamples); 
static void tof_load_default_settings   (void);
static void pintrigger_callback         (void* arg);
static void pintrigger_tx               (void* arg);
static void sram_full_callback          (void* arg);
static void slave_id_change_notify      (modbus_disco_list_t* list,modbus_slave_t* slave);
static void assign_pairs                (tof_t* tof);

static void state_set (tof_state_t state);
static tof_state_t state_get (void);

static void do_state_idle(tof_t* tof);
static void do_state_disco_start(tof_t* tof);
static void do_state_disco_wait(tof_t* tof);
static void do_state_delay_start(tof_t* tof);
static void do_state_delay_wait(tof_t* tof);
static void do_state_set_gains(tof_t* tof);
static void do_state_tx_arm(tof_t* tof);
static void do_state_tx_wait(tof_t* tof);
static void do_state_tx_trigger(tof_t* tof);
static void do_state_rx_wait(tof_t* tof);
static void do_state_rx(tof_t* tof);
static void do_state_get_vars(tof_t* tof);
static void do_state_log(tof_t* tof);
static void do_state_change_dir(tof_t* tof);

extern bool tof_setup(void)
{
    memset(&tof,0,sizeof(tof_t));
    
    tof.xduc_reg_map = xduc_reg_map;
    tof.xduc_nreg = XDUC_HOLDING_NREGS;
    tof.xduc_start_reg = XDUC_HOLDING_START;

    tof_load_default_settings();

    tof.modbus_ctx = modbus_new_rtu(    
                                    RS485_UART_0,
                                    &gpio_rs485_dir,
                                    RS485_SPEED_0_DEF,'N',8,1
                                );

    modbus_disco_master_setup(
                                tof.modbus_ctx,
                                MAX_DISCO_DELAY,
                                DISCO_REFRESH_PERIOD,
                                slave_id_change_notify
                            );

    hw_xduc_sram_dma_setup(sram_full_callback,&tof);

    hw_pintrigger_setup(    
                            &tof.pintrigger,
                            &gpio_rs485_fire,
                            pintrigger_callback,
                            pintrigger_tx,
                            &tof
                        );


    return true;
}

extern void tof_run(void)
{
    hw_xduc_sram_dma_service();
    hw_pintrigger_service(&tof.pintrigger);
    modbus_disco_master_service();

    switch(state_get())
    {
        default:
        case state_disco_start:     do_state_disco_start(&tof);     break;
        case state_disco_wait:      do_state_disco_wait(&tof);      break;
        case state_idle:            do_state_idle(&tof);            break;
        case state_delay_start:     do_state_delay_start(&tof);     break;
        case state_delay_wait:      do_state_delay_wait(&tof);      break;
        case state_set_gains:       do_state_set_gains(&tof);       break;
        case state_tx_arm:          do_state_tx_arm(&tof);          break;
        case state_tx_wait:         do_state_tx_wait(&tof);         break;
        case state_tx_trigger:      do_state_tx_trigger(&tof);      break;
        case state_rx_wait:         do_state_rx_wait(&tof);         break;
        case state_rx:              do_state_rx(&tof);              break;
        case state_get_vars:        do_state_get_vars(&tof);        break;
        case state_log:             do_state_log(&tof);             break;
        case state_change_dir:      do_state_change_dir(&tof);      break;
    }
}

static void state_set(tof_state_t state)
{
    tof.state = state;
}

static tof_state_t state_get(void)
{
    return tof.state;
}

static void do_state_disco_start(tof_t* tof)
{
    tof->time_start=jiffies();
    tof->disco_complete=false;
    modbus_disco_master_reset();
    modbus_disco_master_start();
    state_set(state_disco_wait);
}

static void do_state_disco_wait(tof_t* tof)
{
    if ( tof->disco_complete )
    {
        state_set(state_idle);
    }
    else
    {
        if ( jiffies() - tof->time_start > MAX_DISCO_TIMEOUT )
        {
            state_set(state_disco_start);
        }
    }
}

static void do_state_idle(tof_t* tof)
{
    assign_pairs(tof);
    tof->list_index=0;
}

static void do_state_delay_start(tof_t* tof)
{
    tof->time_start = jiffies();
}

static void do_state_delay_wait(tof_t* tof)
{
    if ( jiffies() - tof->time_start > MEASURE_PERIOD )
    {
        state_set(state_set_gains);
    }
}

static void do_state_set_gains(tof_t* tof)
{
    uint16_t regs[3];
    regs[0] = tof->xduc_tx_duration;             // XDUC_REG_TX_DURATION
    regs[1] = (uint16_t)tof->xduc_tx_power;      // XDUC_REG_TX_GAIN
    regs[2] = tof->xduc_rx_gain;                 // XDUC_REG_RX_GAIN
    
    /* set the slave modbus id */
    modbus_set_slave( tof->modbus_ctx, modbus_disco_list_at( tof->list, tof->list_index )->slave_id );

    /* write the gains */
    tof->rc =  modbus_write_registers( tof->modbus_ctx, XDUC_REG_TX_DURATION, 3, regs );
    if ( tof->rc >= 0 )
        state_set( state_tx_arm );
}

static void do_state_tx_arm(tof_t* tof)
{
    tof->rc = modbus_write_register(tof->modbus_ctx, XDUC_REG_CTRL, XDUC_REG_CTRL_ARM | ( XDUC_ARM_DELAY_MS & XDUC_REG_CTRL_ARM_MS ) );
    if ( tof->rc >= 0 )
    {
        hw_pintrigger_arm(&tof->pintrigger);
        tof->time_start = jiffies();
        state_set(state_tx_wait);
    }           
    else
    {
        state_set(state_log); 
    }
}

static void do_state_tx_wait(tof_t* tof)
{
    if ( jiffies() - tof->time_start > (XDUC_ARM_DELAY_MS/2) )
        state_set(state_tx_trigger);       
}

static void do_state_tx_trigger(tof_t* tof)
{
    hw_pintrigger_trigger(&tof->pintrigger);
    tof->time_start = jiffies();
    state_set(state_rx_wait);
}

static void do_state_rx_wait(tof_t* tof)
{
    if ( jiffies() - tof->time_start > 500 )
    {
        /* @FIXME RX timeout */
        state_set(state_log);
    }
    if ( piezo_rx_complete() )
        state_set(state_rx);
    hw_pintrigger_disarm(&tof->pintrigger);

}

static void do_state_rx(tof_t* tof)
{
    // tof->calc_callback(&tof->capture);
    state_set(state_get_vars);

}

static void do_state_get_vars(tof_t* tof)
{
    {
        // uint16_t* serial = tof_get_serial();
        // tof->rc = modbus_read_registers(modbus, tof->xduc_start_reg, tof->xduc_nreg-1, tof->xduc_reg_map);
        // if ( tof->capture.dir == tof_dir_yx )
        // {
        //     modbus_slave_register_map_put( HOLD_REG_AFX_X_SERIAL_NO_0, serial[0] );
        //     modbus_slave_register_map_put( HOLD_REG_AFX_X_SERIAL_NO_1, serial[1] );
        //     modbus_slave_register_map_put( HOLD_REG_AFX_X_SERIAL_NO_2, serial[2] );
        //     modbus_slave_register_map_put( HOLD_REG_AFX_X_SERIAL_NO_3, serial[3] );
        //     modbus_slave_register_map_put( HOLD_REG_AFX_X_SERIAL_NO_4, serial[4] );
        //     modbus_slave_register_map_put( HOLD_REG_AFX_X_SERIAL_NO_5, serial[5] );
        // }
        // else
        // {
        //     modbus_slave_register_map_put( HOLD_REG_AFX_Y_SERIAL_NO_0, serial[0] );
        //     modbus_slave_register_map_put( HOLD_REG_AFX_Y_SERIAL_NO_1, serial[1] );
        //     modbus_slave_register_map_put( HOLD_REG_AFX_Y_SERIAL_NO_2, serial[2] );
        //     modbus_slave_register_map_put( HOLD_REG_AFX_Y_SERIAL_NO_3, serial[3] );
        //     modbus_slave_register_map_put( HOLD_REG_AFX_Y_SERIAL_NO_4, serial[4] );
        //     modbus_slave_register_map_put( HOLD_REG_AFX_Y_SERIAL_NO_5, serial[5] );
        // }
    }
    state_set(state_log);                
}

static void do_state_log(tof_t* tof)
{
    // if ( tof->rc < 0 )
    //     led_blink(tof->status_led, 2, BLINK_RATE_LO );
    state_set(state_change_dir);

}

static void do_state_change_dir(tof_t* tof)
{
    // tof->capture.dir=(tof->capture.dir==tof_dir_xy)?tof_dir_yx:tof_dir_xy;
    state_set(state_idle);
}

static void sram_full_callback(void* arg)
{
     tof_t* tof = (tof_t*)arg;
       /* NOP */

}

static void pintrigger_callback(void* arg)
{
    tof_t* tof = (tof_t*)arg;
    /* NOP */
}

static void pintrigger_tx(void* arg)
{
    tof_t* tof = (tof_t*)arg;
    /* NOP */
}

static void rx_capture_callback	(uint16_t* capture,size_t nsamples)
{
    /* NOP */
}

static void tof_load_default_settings(void)
{
    tof.xduc_tx_power       = DEFAULT_XDUC_TX_POWER;
    tof.xduc_tx_duration    = DEFAULT_XDUC_TX_DURATION;
    tof.xduc_rx_gain        = DEFAULT_XDUC_RX_GAIN;
}

extern void tof_set_rx_gain(uint16_t xduc_rx_gain)
{
    tof.xduc_rx_gain = xduc_rx_gain;
}

extern void tof_set_tx_duration(uint16_t xduc_tx_duration)
{
    tof.xduc_tx_duration = xduc_tx_duration;
}

extern void tof_set_tx_gain(float xduc_tx_power)
{
    tof.xduc_tx_power = xduc_tx_power;
}

extern uint16_t tof_get_rx_gain(void)
{
    return tof.xduc_rx_gain;
}

extern uint16_t tof_get_tx_duration (void)
{
    return tof.xduc_tx_duration;
}

extern float tof_get_tx_gain (void)
{
    return tof.xduc_tx_power;
}

extern uint16_t* tof_get_serial(void)
{
    return &tof.xduc_reg_map[XDUC_REG_AFX_SERIAL_NO_0-tof.xduc_start_reg];
}

/**
 * @brief Notify callback when a slave is discovered or the slave list is complete.
 * @param list The list of slaves 
 * @param slave The current slave, or NULL if the list is complete. 
 */
static void slave_id_change_notify(modbus_disco_list_t* list,modbus_slave_t* slave)
{
    if ( slave == NULL )
    {
        tof.list = list;
        tof.disco_complete=true;
    }
    else
    {
        tof.list = NULL;
        tof.disco_complete=false;
    }
}

static void assign_pairs(tof_t* tof)
{
    for(int index=0; index < modbus_disco_list_count(tof->list); index++)
    {
        modbus_slave_t* slave = modbus_disco_list_at(tof->list,index);
        xfprintf( xstderr, "[%d,%s]\n",
                            slave->slave_id,
                            serialno_get_hex(&slave->serialno));

        /**
         * @NOTE Stupid simple assign odds and evens as pairs
         * @FIXME - make it a config thing
         */
        if (index+1 < modbus_disco_list_count(tof->list) && (index&1)==0 )
        {
            modbus_slave_t* even_slave = modbus_disco_list_at(tof->list,index+1);
            even_slave->arg = slave;
            slave->arg = even_slave;
        }
    }
}
