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
#include "tof-detect.h"
#include "registers.h"
#include <stdlib.h>
#include <modbus-slave-register-map.h>
#include <linearmap.h>

static float microseconds(tof_capture_t* capture,float x);

extern void tof_detect_callback(tof_capture_t* capture)
{
    uint16_t detect_p_amp = (uint16_t)linearmap((float)modbus_slave_register_map_get( HOLD_REG_DETECT_AMPLITUDE ), 0.0f, 100.0f, 2048.0f, 4095.0f);
    uint16_t detect_n_amp = (uint16_t)linearmap((float)modbus_slave_register_map_get( HOLD_REG_DETECT_AMPLITUDE ), 0.0f, 100.0f, 2048.0f, 0.0f);
    int first=-1;

    capture->index=0;

    for( uint32_t x=0; x < capture->storage_size; x++ )
    {
        int val = capture->storage[x];

        if ( val >= detect_p_amp /* 0xC00 */ || val <= detect_n_amp )
        {
            x -= PIEZO_RX_DMA_BLOCK_SIZE;  /** @FIXME Double-buffer issue? */
            first = x;
            break;
        }
    }
    
    ++capture->cycle_count;

    if ( first >= 0 )
    {
        float tof = microseconds(capture,first);
        capture->tof[(int)capture->dir] = tof;

        if ( modbus_slave_register_map_get( HOLD_REG_WAVEFORMS ) > 0 )
        {
            capture->index = first+PIEZO_RX_DMA_BLOCK_SIZE;
            modbus_slave_register_map_put( HOLD_REG_WAVEFORMS, modbus_slave_register_map_get( HOLD_REG_WAVEFORMS ) - 1 );
        }
    }
    else
    {
        ++capture->fault_count;
    }
}

static float microseconds(tof_capture_t* capture,float x)
{
    return x * capture->usec_per_sample;
}
