 
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
#ifndef _SETTINGS_H_
#define _SETTINGS_H_

#include <board.h>
#include <led.h>
#include <average.h>
#include "registers.h"

#ifdef __cplusplus
extern "C"
{
#endif

#define HOLD_REG_ATTR_RD        0x01
#define HOLD_REG_ATTR_WR        0x02
#define HOLD_REG_ATTR_RW        (HOLD_REG_ATTR_RD|HOLD_REG_ATTR_WR)
#define HOLD_REG_ATTR_32BIT     0x10
#define HOLD_REG_ATTR_PERSIST   0x80

typedef struct _settings_sesist_t 
{
    uint16_t    holding_registers[REG_HOLDING_NREGS];
    uint8_t     holding_register_attr[REG_HOLDING_NREGS];
} settings_persists_t;

typedef struct _settings_t_ 
{
    settings_persists_t persists;
    bool                dirty;
    bool                load;
    bool                button_pressed;
    jiffies_t           button_press_start;
    led_t*              status_led;
} settings_t;

extern void      settings_setup       (settings_t* settings, led_t* status_led);
extern bool      settings_service     (settings_t* settings);
extern void      settings_set_dirty   (settings_t* settings, bool dirty);
extern bool      settings_get_dirty   (settings_t* settings);
extern uint16_t* settings_get_holding (settings_t* settings);
extern uint16_t  settings_get_nholding(settings_t* settings);
extern int       settings_set_baud    (settings_t* settings,uint32_t baud);
extern uint32_t  settings_get_baud    (settings_t* settings);
extern int       settings_set_id      (settings_t* settings,uint8_t id);
extern uint8_t   settings_get_id      (settings_t* settings);

extern bool      settings_button_pressed(void);

#ifdef __cplusplus
}
#endif

#endif
