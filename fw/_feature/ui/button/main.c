/******************************************************************************
******************************************************************************/
#include <board.h>
#include <delay.h>
#include <led.h>
#include <button.h>
#include <hw_gpio.h>

static led_t 	led;
static button_t button;

static void button_callback( button_event_t button_event );

extern void feature_main(void* arg)
{
	led_setup(&led,&gpio_led0,LED0_POLARITY);
	button_setup(&button,&gpio_btn0,BTN0_POLARITY,button_callback);

	for( ;; )
	{
		led_service(&led);
		button_service(&button);
	}
}

static void button_callback(button_event_t button_event)
{
	switch ( button_event)
	{
		case button_down:
			led_on(&led);
			break;
		case button_up:
			led_off(&led);
			break;
		case button_held:
			led_blink(&led,48,32);
			break;
		case button_double_click:
			led_blink(&led,8,128);
			break;
	}
}
