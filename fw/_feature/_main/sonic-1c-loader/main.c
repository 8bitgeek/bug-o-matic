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
#include <board.h>
#include <hw_gpio.h>
#include <ff.h>
#include <srecreader.h>
#include <xprintf.h>
#include <hw_pin.h>
#include <bss_init.h>
#include <led.h>

#define READ_MAX_LINE   132    

typedef void (*entry_point_t)(void); 

typedef enum 
{
    boot_idle=0,
    boot_loader_mount,
    boot_loader_open,
    boot_flash_unlock,
    boot_flash_erase,
    boot_flash_setup,
    boot_loader_loading,
    boot_loader_complete,
    boot_fail,
} boot_state_t;

typedef struct 
{
    boot_state_t    state;
    char            read_buf[READ_MAX_LINE+sizeof(uint32_t)];
    entry_point_t   entry_point;
    srec_reader_t   reader;
    FATFS           filesystem;
	FIL             fil;
	FRESULT         rc;
    bool            is_mounted;
    led_t           led;
} loader_t;

static loader_t loader;

static int  s19_store_fn     ( srec_reader_t* srec_reader);  
static int  s19_fault_fn     ( srec_reader_t* srec_reader);  
static void loader_mount     ( void );
static void loader_open      ( void );
static int  loader_loading   ( void );
static void loader_complete  ( void );
static void bootstrap_service( void );
static void system_reset     ( void );
static void flash_unlock     ( void );
static void flash_lock       ( void );
static bool flash_locked     ( void );
static bool flash_busy       ( void );
static void flash_erase      ( void );
static void flash_setup      ( void );
static void flash_write      ( uint32_t addr, uint32_t* data );
static void boot_memset      ( void* dst,uint8_t ch,size_t size );
static void boot_delay       ( int delay );
static void boot_wait_busy   ( void );
static void out_ch           ( int ch );

extern int main(int argc, char* arv[])
{   
    _bss_init();
    hw_pin_setup();
    xdev_out(out_ch);
    boot_memset(&loader,0,sizeof(loader_t));
    led_setup(&loader.led,&gpio_led0,LED0_POLARITY);

    for(;;)
    {
        led_blink(&loader.led,LED_BLINK_FOREVER,BLINK_RATE_HI/2);
        while( loader.state != boot_fail )
        {
            led_service(&loader.led);
            bootstrap_service();
            if ( loader.state == boot_idle )
                loader.state = boot_loader_mount;
        }
        led_blink(&loader.led,LED_BLINK_FOREVER,BLINK_RATE_LO*2);
        for(;;)
        {
            led_service(&loader.led);
        }
    }
    return 0;
}

static void bootstrap_service(void)
{
    switch( loader.state )
    {
        default:
        case boot_idle: 
            #if BOOSTRAP_DEBUG
                xprintf("boot_idle\n");
            #endif
            break;
        case boot_loader_mount:
            #if BOOSTRAP_DEBUG
                xprintf("boot_loader_mount\n");
            #endif
            loader_mount();
            loader.state = ( loader.is_mounted ) ? boot_loader_open : boot_fail;
            break;
        case boot_loader_open:
            #if BOOSTRAP_DEBUG
                xprintf("boot_loader_open\n");
            #endif
            loader_open();
            loader.state = ( loader.rc == FR_OK ) ? boot_flash_unlock : boot_fail;
            break;
        case boot_flash_unlock:
            #if BOOSTRAP_DEBUG
                xprintf("boot_flash_unlock\n");
            #endif
            flash_unlock();
            loader.state = ( !(FLASH->CR & FLASH_CR_LOCK) ) ? boot_flash_erase : boot_fail;
            break;
        case boot_flash_erase:
            #if BOOSTRAP_DEBUG
                xprintf("boot_flash_erase\n");
            #endif
            flash_erase();
            loader.state = ( !(FLASH->CR & FLASH_CR_LOCK) ) ? boot_flash_setup : boot_fail;
            break;
        case boot_flash_setup:
            #if BOOSTRAP_DEBUG
                xprintf("boot_flash_setup\n");
            #endif
            flash_setup();
            loader.state = ( !(FLASH->CR & FLASH_CR_LOCK) ) ? boot_loader_loading : boot_fail;
            break;
        case boot_loader_loading:
            loader.state = loader_loading() <= 0 ? boot_loader_complete : boot_loader_loading;
            break;
        case boot_loader_complete:
            #if BOOSTRAP_DEBUG
                xprintf("boot_loader_complete\n");
            #endif
            loader_complete();
            system_reset();
            break;
        case boot_fail:
            #if BOOSTRAP_DEBUG
                xprintf("boot_fail loader.rc=%d flash_locked=%d\n",loader.rc,flash_locked());
            #endif
            system_reset();
            break;
    }
}

static void loader_mount( void )
{
    loader.rc = f_mount(&loader.filesystem, "", 0);
    loader.is_mounted = (loader.rc == FR_OK);
}

static void loader_open( void )
{
    if ( (loader.rc = f_open( &loader.fil, BOOT_FIRMWARE, FA_READ )) == FR_OK )
    {
        srec_reader_init( 
                        &loader.reader,
                        &loader.fil,
                        NULL,
                        s19_store_fn,
                        NULL,
                        s19_fault_fn,
                        loader.read_buf,
                        READ_MAX_LINE-1,
                        NULL
                        );
    }
}

static int loader_loading( void )
{
    return srec_reader_read_once(&loader.reader);
}

static void loader_complete( void )
{
    f_close( &loader.fil );
}

static void system_reset( void )
{
    NVIC_SystemReset();
    for(;;);
}   

static void boot_memset(void* dst,uint8_t ch,size_t size)
{
    uint8_t* p = (uint8_t*)dst;
    while(size--)
        *p = ch;
}

static void boot_delay( int delay )
{
    static volatile int x;
    static volatile int y;
    for(x=0; x < delay; x++)
    {
        for(y=0; y < 10000; y++);
    }
}

static void boot_wait_busy( void )
{
    while ( flash_busy() );
}

/****************************************************************************
 * S19 Record Callbacks
 ****************************************************************************/

/* store payload callback */
static int s19_store_fn( srec_reader_t* srec_reader)   
{
    srec_result_t* record = &srec_reader->record;
    uint32_t addr = record->address;
    
    #if BOOSTRAP_DEBUG
        xprintf( "FLASH: %08X: ", addr );
    #endif

    for( uint16_t n=0; n < record->length; n+=sizeof(uint32_t) )
    {
        uint32_t* pdata = ((uint32_t*)&record->data[n]);
        
        boot_wait_busy();

        #if BOOSTRAP_DEBUG
            xprintf( "%02X%02X%02X%02X", 
                record->data[n],record->data[n+1],record->data[n+2],record->data[n+3] );
        #endif

        flash_write( addr, pdata );
        addr += 4;
    }
    boot_wait_busy();
    
    #if BOOSTRAP_DEBUG
        xprintf( "\n" );
    #endif

    return 0;
}

/* s19 fault callback */
static int  s19_fault_fn( srec_reader_t* srec_reader)
{
    #if BOOSTRAP_DEBUG
        srec_result_t* record = &srec_reader->record;
        xprintf( "fault %d\n", record->record );
    #endif
    return 0;
}


/****************************************************************************
 * FLASH
 ****************************************************************************/

static void flash_unlock( void )
{
    FLASH->KEYR = FLASH_KEY1;
    FLASH->KEYR = FLASH_KEY2;
}

static void flash_lock( void )
{
    FLASH->CR |= FLASH_CR_LOCK;
}

static bool flash_locked( void )
{
    return (FLASH->CR & FLASH_CR_LOCK) != 0;
}

static bool flash_busy( void )
{
    return (FLASH->SR & FLASH_SR_BSY) != 0;
}

static void flash_erase( void )
{
    boot_wait_busy();
    FLASH->CR |= FLASH_CR_MER;
    FLASH->CR |= FLASH_CR_STRT;
    boot_wait_busy();
}

static void flash_setup( void )
{
    FLASH->CR &= CR_PSIZE_MASK;
    FLASH->CR |= FLASH_PSIZE_WORD;
}

static void flash_write(uint32_t addr, uint32_t* data)
{
    FLASH->CR |= FLASH_CR_PG;
    *(__IO uint32_t*)addr = *data;
    __DSB();
}

/****************************************************************************
 * UART
 ****************************************************************************/

static void out_ch(int ch)
{
    while ( !(USART1->ISR & USART_ISR_TXE) );
    USART1->TDR = (uint8_t)(ch&0xFF);
}