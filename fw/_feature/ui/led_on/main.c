/******************************************************************************
******************************************************************************/
#include <board.h>
#include <delay.h>
#include <led.h>

static led_t led;

extern void feature_main(void* arg)
{
	led_setup(&led,&gpio_led0,LED0_POLARITY);
	led_on(&led);

	for( ;; )
	{
	}
}
