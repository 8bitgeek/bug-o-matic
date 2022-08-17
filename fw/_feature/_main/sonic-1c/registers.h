 
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
#ifndef _MAIN_REGISTER_MAP_H_
#define _MAIN_REGISTER_MAP_H_

#include <board.h>
#include <diskio.h>
#include <ff.h>

#ifdef __cplusplus
extern "C"
{
#endif

/******************************************************************************
 * @brief Holding Register Map Constants 
******************************************************************************/
#define REG_HOLDING_START               (0)
#define REG_HOLDING_NREGS		        (1000)

#define HOLD_REG_STATUS_NULL_READING    (1)

/**/
typedef enum _modbus_reg_addr_t_
{
    HOLD_REG_FW_VERSION=1,              /* uint16_t Firmware Version */
    HOLD_REG_TOF_X,                     /* uint16_t X time-of-flight in microseconds */
    HOLD_REG_TOF_Y,                     /* uint16_t Y time-of-flight in microseconds */
    HOLD_REG_VELOCITY,                  /* uint16_t Velocity in mm/sec */
    HOLD_REG_LOW_PASS_FILTER,           /* uint16_t Low pass (averaging) filter (0-100) */
    HOLD_REG_REVERSE,                   /* uint16_t Reverse Flow Direction (bool) */
    HOLD_REG_RX_GAIN,                   /* uint16_t Receiver gain 0-14 */
    HOLD_REG_TX_GAIN,                   /* uint16_t Transmitter gain 0-100 */
    HOLD_REG_DURATION,                  /* uint16_t Transmitter cycles 1-32 */
    HOLD_REG_DETECTOR,                  /* uint16_t Simple, Envelope, ZX (0-2) */
    HOLD_REG_DETECT_AMPLITUDE,          /* uint16_t Detection Amplitude (0-100) */
    HOLD_REG_DETECT_ZERO_OFFSET,        /*  int16_t Detection Offset (*1000) */
    HOLD_REG_COUNTER,                   /* uint16_t Counter / Heartbeat */
    HOLD_REG_STATUS,                    /* uint16_t bit#0=null reading/obstruction */
    HOLD_REG_FAULT_COUNT,               /* uint16_t fault counter */
    HOLD_REG_DAYS,                      /* uint16_t Days Uptime */
    HOLD_REG_HOURS,                     /* uint16_t Hours Uptime */
    HOLD_REG_MINUTES,                   /* uint16_t Minutes Uptime */
    HOLD_REG_SECONDS,                   /* uint16_t Seconds Uptime */
    HOLD_REG_TOF_COUNT,                 /* uint16_t Number of TOF Measurements */
    HOLD_REG_WAVEFORMS,                 /* uint16_t Number Waveform Captures */
    /**/
    HOLD_REG_BASELINESET      =50,      /* uint16_t Baseline in mm (WRITE) */
    HOLD_REG_BASELINEGET,               /* uint16_t Baseline in mm (READ) */
    HOLD_REG_FACE2FACESET,              /* uint16_t Face2Face in mm (WRITE) */
    HOLD_REG_FACE2FACEGET,              /* uint16_t Face2Face in mm (READ) */
    /**/
    HOLD_REG_V_M_SEC_FLOAT32  =100,     /* float32 Velocity in meters/sec */
    HOLD_REG_V_FPM_FLOAT32    =102,     /* float32 Volume in Feet per Minute */
    HOLD_REG_V_CFM_FLOAT32    =104,     /* float32 Volume in Cubic Feet per Minute */
    HOLD_REG_V_KCFM_FLOAT32   =106,     /* float32 Volume in K Cubic Feet per Minute */
    HOLD_REG_V_CMS_FLOAT32    =108,     /* float32 Volume in K Cubic Meters per Second */
    HOLD_REG_V_DELTAT_FLOAT32 =110,     /* float32 Delta Time in microseconds */
    /**/
    HOLD_REG_SQUARE_METERS    =120,     /* float32 Area in square meters */
    /**/
    HOLD_REG_CORRECTION_FACTOR=150,     /* float32 Correction Factor meters/sec */
    HOLD_REG_ZERO_CUTOFF      =152,     /* float32 Zero cut-off meters/sec */
    /**/
    HOLD_REG_BAUD_RATE_100    =200,     /* uint16_t Baud Rate / 100 */
    HOLD_REG_SLAVE_ID,                  /* uint16_t Slave ID */
    /**/
    HOLD_REG_AQ_SERIAL_NO_0   =300,     /* Serial_Number[0] */
    HOLD_REG_AQ_SERIAL_NO_1,            /* Serial_Number[1] */
    HOLD_REG_AQ_SERIAL_NO_2,            /* Serial_Number[2] */
    HOLD_REG_AQ_SERIAL_NO_3,            /* Serial_Number[3] */
    HOLD_REG_AQ_SERIAL_NO_4,            /* Serial_Number[4] */
    HOLD_REG_AQ_SERIAL_NO_5,            /* Serial_Number[5] */
    /**/
    HOLD_REG_AFX_X_SERIAL_NO_0,         /* XDUC X Serial_Number[0] */
    HOLD_REG_AFX_X_SERIAL_NO_1,         /* XDUC X Serial_Number[1] */
    HOLD_REG_AFX_X_SERIAL_NO_2,         /* XDUC X Serial_Number[2] */
    HOLD_REG_AFX_X_SERIAL_NO_3,         /* XDUC X Serial_Number[3] */
    HOLD_REG_AFX_X_SERIAL_NO_4,         /* XDUC X Serial_Number[4] */
    HOLD_REG_AFX_X_SERIAL_NO_5,         /* XDUC X Serial_Number[5] */
    /**/
    HOLD_REG_AFX_Y_SERIAL_NO_0,         /* XDUC Y Serial_Number[0] */
    HOLD_REG_AFX_Y_SERIAL_NO_1,         /* XDUC Y Serial_Number[1] */
    HOLD_REG_AFX_Y_SERIAL_NO_2,         /* XDUC Y Serial_Number[2] */
    HOLD_REG_AFX_Y_SERIAL_NO_3,         /* XDUC Y Serial_Number[3] */
    HOLD_REG_AFX_Y_SERIAL_NO_4,         /* XDUC Y Serial_Number[4] */
    HOLD_REG_AFX_Y_SERIAL_NO_5,         /* XDUC Y Serial_Number[5] */
    /**/
    HOLD_REG_XFR_BYTES_CTRL   =500,     /* XFER Control bits */
    HOLD_REG_XFR_BYTES_STATUS,          /* XFER Status bits */
    HOLD_REG_XFR_BYTES_PSIZE,           /* Number of bytes per full page */
    HOLD_REG_XFR_BYTES_READY,           /* Number of XFER bytes ready */
    HOLD_REG_XFR_BYTES,                 /* Payload bytes (LSB is first byte) */

} modbus_reg_addr_t;

#define XFER_CTRL_DL_WAV    0x0001      /* xfer request download wave */
#define XFER_CTRL_DIR_MSK   0x0002      /* 0 = x->y, 1 = y->x */

#define XFER_STATUS_BUSY    0x0001      /* xfer is busy */
#define XFER_STATUS_EOF     0x0002      /* xfer is end-of-file (EOF) */
#define XFER_STATUS_FLT_MSK 0xF000      /* xfer fault code */
#define XFER_STATUS_FLT_IO  0x1000      /* I/O fault */
#define XFER_STATUS_FLT_TO  0x2000      /* time-out fault */

#ifdef __cplusplus
}
#endif

#endif
