/******************************************************************************
******************************************************************************/
#include <board.h>
#include <delay.h>
#include <led.h>

static led_t led;

extern void feature_main(void* arg)
{
	led_setup(&led,&gpio_led0,LED0_POLARITY);

	fprintf(stderr,"type characters and observe echo...\n");
	for( ;; )
	{
		int ch = fgetc(stderr);

		if ( ch >= 0 )
		{
			led_on(&led);
			msdelay(1);
			fputc(ch,stderr);
			led_off(&led);
		}
	}
}
