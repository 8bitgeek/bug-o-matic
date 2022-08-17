/******************************************************************************
******************************************************************************/
#include <board.h>
#include <delay.h>
#include <hw_gpio.h>

#define DELAY_US	10

extern void feature_main(void* arg)
{	
	for( ;; )
	{
		hw_gpio_set(&gpio_test0);
		usdelay(DELAY_US);
		hw_gpio_reset(&gpio_test0);
		usdelay(DELAY_US);
	}
}
