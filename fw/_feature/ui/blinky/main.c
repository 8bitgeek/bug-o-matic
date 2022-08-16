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

/****************************************************************************/
/*																			*/
/*-- # This is a comment in 'cook version 3' meta data						*/
/*-- # Comments get transposed to the output build snippets					*/
/*																			*/
/*-- # Inherit the properties of another feature.                           */
/*-- .inherit caribou 														*/
/*																			*/
/*-- # Add additional source depencencies.                                  */
/*-- .use	delay															*/
/*-- .use	led																*/
/*																			*/
/*-- # Subtract unnessesary source dependencies.							*/
/*-- .unuse	out5															*/
/*			 																*/
/*-- # Preprocessor definitions												*/
/*-- .define LED0_POLARITY=false											*/
/*-- .define FEATURE_DELAY=64												*/
/*																			*/
/****************************************************************************/

#include <board.h>
#include <delay.h>
#include <led.h>

static led_t led;

extern void feature_main(void* arg)
{
	led_setup(&led,&gpio_led0,LED0_POLARITY);
	led_blink(&led,LED_BLINK_FOREVER,FEATURE_DELAY);
	
	for( ;; )
	{
		led_service(&led);
	}
}
