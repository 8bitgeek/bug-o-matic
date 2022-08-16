 
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
#ifndef _TOF_UNITS_H_
#define _TOF_UNITS_H_

#include <board.h>
#include "tof-run.h"

#ifdef __cplusplus
extern "C"
{
#endif

typedef enum 
{
    ToMetersPerSecond,
    ToKCubicFeetPerMinute,
    ToCubicFeetPerMinute,
    ToCubicMetersPerSecond,
    ToFeetPerMinute,
    ToMicroSeconds,
} to_units_t;


/**
 * @brief Convert intout velocity to the selected units of measure.
 * @param to_units The destination conversion enum
 * @param v_meters_per_second The measured millimeters per millisecond.
 * @param square_meters The area in square meters.
 * @param microseconds The average time of flight in microseconds.
 */
extern double convert_units(to_units_t to_units, double v_meters_per_second, double square_meters, double microseconds);


#ifdef __cplusplus
}
#endif


#endif
