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
#include "settings.h"
#include <hw_gpio.h>

#define SETTINGS_DEFAULT_BAUD   9600
#define SETTINGS_DEFAULT_ID     1
#define SETTINGS_FN             "SETTINGS.MAP"
#define SETTINGS_BAUD_FN        "BAUD.TXT"
#define SETTINGS_ID_FN          "ID.TXT"

static bool settings_load(settings_t* settings);
static bool settings_save(settings_t* settings);
static void settings_button_service(settings_t* settings);
static int  settings_load_int(const char* fn);
static int  settings_store_int(const char* fn,int i);
static void clean_buf(char* buf, int sz);
static void settings_set_attrs(settings_t* settings);


extern void settings_setup(settings_t* settings,led_t* status_led)
{
    memset(settings,0,sizeof(settings_t));
    settings_set_attrs(settings);

    /* If settings button is pressed then skip auto-loading the settings file, and run with defaults */
    settings->load=!settings_button_pressed();

    settings->status_led = status_led;
}

static void settings_set_attrs(settings_t* settings)
{
    settings->persists.holding_register_attr[HOLD_REG_FW_VERSION]           = HOLD_REG_ATTR_RD;
    settings->persists.holding_register_attr[HOLD_REG_TOF_X]                = HOLD_REG_ATTR_RD;
    settings->persists.holding_register_attr[HOLD_REG_TOF_Y]                = HOLD_REG_ATTR_RD;
    settings->persists.holding_register_attr[HOLD_REG_VELOCITY]             = HOLD_REG_ATTR_RD;
    settings->persists.holding_register_attr[HOLD_REG_LOW_PASS_FILTER]      = HOLD_REG_ATTR_RW|HOLD_REG_ATTR_PERSIST;
    settings->persists.holding_register_attr[HOLD_REG_REVERSE]              = HOLD_REG_ATTR_RW|HOLD_REG_ATTR_PERSIST;
    settings->persists.holding_register_attr[HOLD_REG_RX_GAIN]              = HOLD_REG_ATTR_RW|HOLD_REG_ATTR_PERSIST;
    settings->persists.holding_register_attr[HOLD_REG_TX_GAIN]              = HOLD_REG_ATTR_RW|HOLD_REG_ATTR_PERSIST;
    settings->persists.holding_register_attr[HOLD_REG_DURATION]             = HOLD_REG_ATTR_RW|HOLD_REG_ATTR_PERSIST;
    settings->persists.holding_register_attr[HOLD_REG_DETECTOR]             = HOLD_REG_ATTR_RW|HOLD_REG_ATTR_PERSIST;
    settings->persists.holding_register_attr[HOLD_REG_DETECT_AMPLITUDE]     = HOLD_REG_ATTR_RW|HOLD_REG_ATTR_PERSIST;
    settings->persists.holding_register_attr[HOLD_REG_DETECT_ZERO_OFFSET]   = HOLD_REG_ATTR_RW|HOLD_REG_ATTR_PERSIST;
    settings->persists.holding_register_attr[HOLD_REG_COUNTER]              = HOLD_REG_ATTR_RD;
    settings->persists.holding_register_attr[HOLD_REG_STATUS]               = HOLD_REG_ATTR_RD;
    settings->persists.holding_register_attr[HOLD_REG_FAULT_COUNT]          = HOLD_REG_ATTR_RD;
    settings->persists.holding_register_attr[HOLD_REG_DAYS]                 = HOLD_REG_ATTR_RD;
    settings->persists.holding_register_attr[HOLD_REG_HOURS]                = HOLD_REG_ATTR_RD;
    settings->persists.holding_register_attr[HOLD_REG_MINUTES]              = HOLD_REG_ATTR_RD;
    settings->persists.holding_register_attr[HOLD_REG_SECONDS]              = HOLD_REG_ATTR_RD;
    settings->persists.holding_register_attr[HOLD_REG_TOF_COUNT]            = HOLD_REG_ATTR_RD;
    settings->persists.holding_register_attr[HOLD_REG_WAVEFORMS]            = HOLD_REG_ATTR_RW;
    /**/
    settings->persists.holding_register_attr[HOLD_REG_BASELINESET]          = HOLD_REG_ATTR_WR|HOLD_REG_ATTR_PERSIST;
    settings->persists.holding_register_attr[HOLD_REG_BASELINEGET]          = HOLD_REG_ATTR_RD;
    settings->persists.holding_register_attr[HOLD_REG_FACE2FACESET]         = HOLD_REG_ATTR_WR|HOLD_REG_ATTR_PERSIST;
    settings->persists.holding_register_attr[HOLD_REG_FACE2FACEGET]         = HOLD_REG_ATTR_RD;
    /**/
    settings->persists.holding_register_attr[HOLD_REG_V_M_SEC_FLOAT32]      = HOLD_REG_ATTR_RD|HOLD_REG_ATTR_32BIT;
    settings->persists.holding_register_attr[HOLD_REG_V_FPM_FLOAT32]        = HOLD_REG_ATTR_RD|HOLD_REG_ATTR_32BIT;
    settings->persists.holding_register_attr[HOLD_REG_V_CFM_FLOAT32]        = HOLD_REG_ATTR_RD|HOLD_REG_ATTR_32BIT;
    settings->persists.holding_register_attr[HOLD_REG_V_KCFM_FLOAT32]       = HOLD_REG_ATTR_RD|HOLD_REG_ATTR_32BIT;
    settings->persists.holding_register_attr[HOLD_REG_V_CMS_FLOAT32]        = HOLD_REG_ATTR_RD|HOLD_REG_ATTR_32BIT;
    settings->persists.holding_register_attr[HOLD_REG_V_DELTAT_FLOAT32]     = HOLD_REG_ATTR_RD|HOLD_REG_ATTR_32BIT;
    /**/
    settings->persists.holding_register_attr[HOLD_REG_SQUARE_METERS]        = HOLD_REG_ATTR_RW|HOLD_REG_ATTR_PERSIST;
    /**/    
    settings->persists.holding_register_attr[HOLD_REG_CORRECTION_FACTOR]    = HOLD_REG_ATTR_RW|HOLD_REG_ATTR_PERSIST;
    settings->persists.holding_register_attr[HOLD_REG_ZERO_CUTOFF]          = HOLD_REG_ATTR_RW|HOLD_REG_ATTR_PERSIST;
    /**/
    settings->persists.holding_register_attr[HOLD_REG_BAUD_RATE_100]        = HOLD_REG_ATTR_RW|HOLD_REG_ATTR_PERSIST;
    settings->persists.holding_register_attr[HOLD_REG_SLAVE_ID]             = HOLD_REG_ATTR_RW|HOLD_REG_ATTR_PERSIST;
    /**/
    settings->persists.holding_register_attr[HOLD_REG_AQ_SERIAL_NO_0]       = HOLD_REG_ATTR_RD;
    settings->persists.holding_register_attr[HOLD_REG_AQ_SERIAL_NO_1]       = HOLD_REG_ATTR_RD;
    settings->persists.holding_register_attr[HOLD_REG_AQ_SERIAL_NO_2]       = HOLD_REG_ATTR_RD;
    settings->persists.holding_register_attr[HOLD_REG_AQ_SERIAL_NO_3]       = HOLD_REG_ATTR_RD;
    settings->persists.holding_register_attr[HOLD_REG_AQ_SERIAL_NO_4]       = HOLD_REG_ATTR_RD;
    settings->persists.holding_register_attr[HOLD_REG_AQ_SERIAL_NO_5]       = HOLD_REG_ATTR_RD;
    /**/
    settings->persists.holding_register_attr[HOLD_REG_AFX_X_SERIAL_NO_0]    = HOLD_REG_ATTR_RD;
    settings->persists.holding_register_attr[HOLD_REG_AFX_X_SERIAL_NO_1]    = HOLD_REG_ATTR_RD;
    settings->persists.holding_register_attr[HOLD_REG_AFX_X_SERIAL_NO_2]    = HOLD_REG_ATTR_RD;
    settings->persists.holding_register_attr[HOLD_REG_AFX_X_SERIAL_NO_3]    = HOLD_REG_ATTR_RD;
    settings->persists.holding_register_attr[HOLD_REG_AFX_X_SERIAL_NO_4]    = HOLD_REG_ATTR_RD;
    settings->persists.holding_register_attr[HOLD_REG_AFX_X_SERIAL_NO_5]    = HOLD_REG_ATTR_RD;
    /**/
    settings->persists.holding_register_attr[HOLD_REG_AFX_Y_SERIAL_NO_0]    = HOLD_REG_ATTR_RD;
    settings->persists.holding_register_attr[HOLD_REG_AFX_Y_SERIAL_NO_1]    = HOLD_REG_ATTR_RD;
    settings->persists.holding_register_attr[HOLD_REG_AFX_Y_SERIAL_NO_2]    = HOLD_REG_ATTR_RD;
    settings->persists.holding_register_attr[HOLD_REG_AFX_Y_SERIAL_NO_3]    = HOLD_REG_ATTR_RD;
    settings->persists.holding_register_attr[HOLD_REG_AFX_Y_SERIAL_NO_4]    = HOLD_REG_ATTR_RD;
    settings->persists.holding_register_attr[HOLD_REG_AFX_Y_SERIAL_NO_5]    = HOLD_REG_ATTR_RD;
}


extern bool settings_service(settings_t* settings)
{
    settings_button_service( settings );

    if ( settings->load )
    {
        if ( settings_load(settings) )
        {
            settings->load=false;
        }
    }

    if ( settings->dirty )
    {
        if ( settings_save(settings) )
        {
            led_blink(settings->status_led, 4, BLINK_RATE_HI );
            settings->dirty=false;
            return true;
        }
        return false;
    }

    return true;
}

static void settings_button_service(settings_t* settings)
{
    if ( settings_button_pressed() )
    {
        if ( !settings->button_pressed )
        {
            settings->button_pressed=true;
            settings->button_press_start = jiffies();
        }

        if ( settings->button_pressed && jiffies() - settings->button_press_start > 4000 )
        {
            settings->button_pressed = false;
            settings_set_dirty(settings,true);
        }
    }
    else
    {
        settings->button_pressed=false;
    }
}

extern bool settings_button_pressed(void)
{
    return !hw_gpio_state(&gpio_btn0);
}

extern void settings_set_dirty(settings_t* settings, bool dirty)
{
    settings->dirty = dirty;
}

extern bool settings_get_dirty(settings_t* settings)
{
    return settings->dirty;
}

extern uint16_t* settings_get_holding (settings_t* settings)
{
    return settings->persists.holding_registers;
}

extern uint16_t settings_get_nholding(settings_t* settings)
{
    return REG_HOLDING_NREGS;
}

extern uint32_t settings_get_baud(settings_t* settings)
{
    uint32_t baud = settings_load_int(SETTINGS_BAUD_FN);
    return baud > 0 ? baud : SETTINGS_DEFAULT_BAUD;
}

extern uint8_t settings_get_id(settings_t* settings)
{
    uint8_t id = (uint8_t)settings_load_int(SETTINGS_ID_FN);
    return id > 0 ? id : SETTINGS_DEFAULT_ID;
}

extern int settings_set_baud(settings_t* settings,uint32_t baud)
{
    if ( baud == 0 )
        baud = SETTINGS_DEFAULT_BAUD;
    return settings_store_int(SETTINGS_BAUD_FN,baud);
}

extern int settings_set_id(settings_t* settings,uint8_t id)
{
    if (id==0)
        id=SETTINGS_DEFAULT_ID;
    return settings_store_int(SETTINGS_ID_FN,id);
}

extern bool settings_load(settings_t* settings)
{
	FIL fil;
	FRESULT rc;

	if ( (rc = f_open( &fil, SETTINGS_FN, FA_READ )) == FR_OK )
	{
        while ( !f_eof(&fil) )
        {
            uint16_t addr;
            UINT bytes_read;
            rc = f_read( &fil, &addr, sizeof(uint16_t), &bytes_read );
            if ( rc == FR_OK )
            {
                uint8_t attr = settings->persists.holding_register_attr[addr];
                rc = f_read( &fil, &settings->persists.holding_registers[addr], sizeof(uint16_t), &bytes_read );
                if ( rc == FR_OK && attr & HOLD_REG_ATTR_32BIT )
                    rc = f_read( &fil, &settings->persists.holding_registers[addr+1], sizeof(uint16_t), &bytes_read );
            }
        }
		f_close( &fil );
	}
    return rc==0;

}

extern bool settings_save(settings_t* settings)
{
	FIL fil;
	FRESULT rc;

	if ( (rc = f_open( &fil, SETTINGS_FN, FA_CREATE_ALWAYS | FA_READ | FA_WRITE )) == FR_OK )
	{
        for( uint16_t addr=0; addr < REG_HOLDING_NREGS; addr++ )
        {
            uint8_t attr = settings->persists.holding_register_attr[addr];
            if ( attr & HOLD_REG_ATTR_PERSIST )
            {
                UINT bytes_written;
                rc = f_write( &fil, &addr, sizeof(uint16_t), &bytes_written );
                if ( rc == FR_OK )
                    rc = f_write( &fil, &settings->persists.holding_registers[addr], sizeof(uint16_t), &bytes_written );
                if ( rc == FR_OK && attr & HOLD_REG_ATTR_32BIT )
                    rc = f_write( &fil, &settings->persists.holding_registers[addr+1], sizeof(uint16_t), &bytes_written );
            }
        }
        f_close( &fil );
	}
    return rc==0;
}

static int  settings_store_int(const char* fn,int i)
{
	FIL fil;
	FRESULT rc;
    static char buf[64];

	if ( (rc = f_open( &fil, fn, FA_CREATE_ALWAYS | FA_READ | FA_WRITE )) == FR_OK )
	{
		UINT bytes_written;
        sprintf(buf,"%d",i);
		rc = f_write( &fil, buf, strlen(buf), &bytes_written );
		if ( rc == FR_OK && bytes_written == strlen(buf) )
		    rc=0;
		f_close( &fil );
	}
    return rc==0;
}


static int settings_load_int(const char* fn)
{
	FIL fil;
	FRESULT rc;
    int baud=(-1);
    static char buf[64];

    memset(buf,0,64);
	if ( (rc = f_open( &fil, fn, FA_READ )) == FR_OK )
	{
        UINT bytes_read;
        rc = f_read( &fil, buf, 63, &bytes_read );
        if ( rc == FR_OK && bytes_read > 0 )
        {
            clean_buf(buf,64);
            baud = atoi(buf);
        }
		f_close( &fil );
	}
    return baud;
}


static void clean_buf(char* buf, int sz)
{
    for( int n=0; n < sz; n++)
    {
        if ( buf[n]<' ' )
            buf[n]='\0';
    }
}