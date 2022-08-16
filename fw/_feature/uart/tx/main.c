/******************************************************************************
******************************************************************************/
#include <board.h>
#include <delay.h>
#include <xprintf.h>

extern void feature_main(void* arg)
{
	for(uint32_t y=0; ;y++)
	{
		xfprintf( xstderr, "%08X: ", y );
		for(int x=' '; x <= '~'; x++)
            xfputc( xstderr, x );
        xfputc( xstderr, '\r' );
        xfputc( xstderr, '\n' );
		msdelay(100);
	}
}
