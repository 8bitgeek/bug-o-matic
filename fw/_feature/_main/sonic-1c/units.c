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
#include "units.h"
#include <stdlib.h>

#define METERS_TO_FEET  3.28084
#define FEET_TO_METERS  0.3048

extern double convert_units(to_units_t to_units, double v_meters_per_second, double square_meters, double microseconds)
{
    double rc;
    switch(to_units)
    {
        default:
        case ToMetersPerSecond:
            rc = v_meters_per_second;
            break;
        case ToKCubicFeetPerMinute:
            rc = ( ( square_meters * METERS_TO_FEET * METERS_TO_FEET ) * ( v_meters_per_second * METERS_TO_FEET ) * 60.0 ) / 1000.0;
            break;
        case ToCubicFeetPerMinute:
            rc = ( square_meters * METERS_TO_FEET * METERS_TO_FEET ) * ( v_meters_per_second * METERS_TO_FEET ) * 60.0;
            break;
        case ToCubicMetersPerSecond:
            rc = square_meters * v_meters_per_second;		
            break;
        case ToFeetPerMinute:
            rc = ( v_meters_per_second * METERS_TO_FEET ) * 60.0;
            break;
        case ToMicroSeconds:
            rc = microseconds;
            break;
    }
    return rc;
}
