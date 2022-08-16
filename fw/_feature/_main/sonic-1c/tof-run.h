 
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
#ifndef TOF_RUN_H_
#define TOF_RUN_H_

#include <board.h>
#include <led.h>

#ifdef __cplusplus
extern "C"
{
#endif

extern bool tof_setup(void);
extern void tof_run  (void);

extern void tof_set_rx_gain     (uint16_t xduc_rx_gain);
extern void tof_set_tx_duration (uint16_t xduc_tx_duration);
extern void tof_set_tx_gain     (float    xduc_tx_power);

extern uint16_t tof_get_rx_gain     (void);
extern uint16_t tof_get_tx_duration (void);
extern float    tof_get_tx_gain     (void);

extern uint16_t* tof_get_serial     (void);

#ifdef __cplusplus
}
#endif

#endif
