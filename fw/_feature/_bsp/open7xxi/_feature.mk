MCU_CHIP	 	= stm32f7xx
MCU_CORE     	= cortex-m7
MCU_FLOAT    	= soft
CORTEX_VER   	= $(ENV_CORTEX_VER)
OPENOCD_TARGET 	= stm32f7x

CFLAGS_COMMON += -fshort-enums -fsigned-char -mlittle-endian -mthumb -mthumb-interwork -mcpu=$(MCU_CORE) -mfloat-abi=$(MCU_FLOAT)
CFLAGS_COMMON += -DUSDELAY_FACTOR=5
CFLAGS_COMMON += -DHSE_VALUE=8000000U 
CFLAGS_COMMON += -DPIEZO_MAX_DAC_PWR=0x0CFF
CFLAGS_COMMON += -DCARIBOU_FLOAT=1 -DSTM32F746xx=1 -DUSART_QUEUE_SZ=512
CFLAGS_COMMON += -DSERIAL_NO_BASE_ADDRESS=0x1FF0F420
CFLAGS_COMMON += -DAFX_MAX_PACKET_SZ=1024
CFLAGS_COMMON += -DDEBUG_UART=0
CFLAGS_COMMON += -DRS485_UART=5
CFLAGS_COMMON += -DRS485_USE_RX_DMA=1 
CFLAGS_COMMON += -DCARIBOU_TEST_VECTOR_BOUNDS=1

CFLAGS_COMMON += -DAFX_TIMING_TX_PORT=GPIOC
CFLAGS_COMMON += -DAFX_TIMING_TX_PIN=6
CFLAGS_COMMON += -DAFX_TIMING_TX_UART=GPIO_AF_USART6

CFLAGS_COMMON += -DFEATURE_MODBUS_MASTER_PORT_0=RS485_UART
CFLAGS_COMMON += -DFEATURE_MODBUS_MASTER_DIR_0=gpio_rs485_dir 
CFLAGS_COMMON += -DFEATURE_MODBUS_MASTER_BAUD_0=115200

# CFLAGS_COMMON += -DDISCO_REFRESH_PERIOD=15*1000*60
CFLAGS_COMMON += -DDISCO_REFRESH_PERIOD=1000*15
CFLAGS_COMMON += -DMIN_DISCO_DELAY=10				# Minimum discovery delay
CFLAGS_COMMON += -DMAX_DISCO_DELAY=64				# Maximum discovery delay

CFLAGS_COMMON += -DFEATURE_DELAY=64
CFLAGS_COMMON += -DLED0_POLARITY=false
CFLAGS_COMMON += -DLED1_POLARITY=false
CFLAGS_COMMON += -DLED2_POLARITY=false
CFLAGS_COMMON += -DBTN0_POLARITY=true

CFLAGS_COMMON += -DPRODUCT_IF_NAME=\"e0\"
CFLAGS_COMMON += -DPRODUCT_HTTP_SERVER_STK_SZ=1024
CFLAGS_COMMON += -DPRODUCT_HTTP_SERVER_NAME=\"httpd\"
CFLAGS_COMMON += -DPRODUCT_HTTP_THREAD_PRIO=1
CFLAGS_COMMON += -DPRODUCT_HTTPD_WD_COUNT=100
CFLAGS_COMMON += -DPRODUCT_HTTP_SEND_BUFFER_SZ=1024
CFLAGS_COMMON += -DPRODUCT_HTTP_PORT=80
CFLAGS_COMMON += -DPRODUCT_HTTP_BACKLOG=16
CFLAGS_COMMON += -DPRODUCT_HTTP_THREAD_STK_SZ=1024
CFLAGS_COMMON += -DPRODUCT_HTTP_SESSION_NAME=\"http\" 
CFLAGS_COMMON += -DPRODUCT_HTTP_SESSION_PRIO=1
CFLAGS_COMMON += -DPRODUCT_HTTP_WD_COUNT=100
CFLAGS_COMMON += -DPRODUCT_I2CSPEED=30
CFLAGS_COMMON += -DPRODUCT_HTTP_ROOT=\"/\"
CFLAGS_COMMON += -DPRODUCT_EEPROM_BPP=16
CFLAGS_COMMON += -DPRODUCT_HOSTNAME=\"sonic\"
CFLAGS_COMMON += -DPRODUCT_VERSION=\"0.1\"
CFLAGS_COMMON += -DPRODUCT_DHCP_THREAD_STACK_SZ=1024
CFLAGS_COMMON += -DPRODUCT_DHCP_THREAD_PRIORITY=1
CFLAGS_COMMON += -DENC28J60_SPI=SPI1
CFLAGS_COMMON += -DENC28J60_USE_DMA=0
CFLAGS_COMMON += -DENC28J60_MAX_PACKET=1500
CFLAGS_COMMON += -DPRODUCT_ETHIF_THREAD_STK_SZ=1024
CFLAGS_COMMON += -DPRODUCT_ETHIF_THREAD_PRIO=1
CFLAGS_COMMON += -DPRODUCT_ETHIN_WD_COUNT=100
CFLAGS_COMMON += -DPRODUCT_DEBUG_SETTINGS=1

ASFLAGS	+= -mlittle-endian -mthumb -mthumb-interwork -mcpu=$(MCU_CORE)

# sources
include ${SRC_ROOT}/_bsp/${TARGET}/_src.mk

include ${SRC_ROOT}/filesystem/_src.mk
