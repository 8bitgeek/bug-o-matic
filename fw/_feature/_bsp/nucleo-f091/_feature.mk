MCU_CHIP	 	= stm32f0xx
MCU_CORE     	= cortex-m0
MCU_FLOAT    	= softfp
CORTEX_VER   	= $(ENV_CORTEX_VER)
OPENOCD_TARGET 	= stm32f0x

# LIBS += $(GCC_ROOT)$(GCC_LIB)/thumb/$(CORTEX_VER)/nofp/libgcc.a
# LIBS += $(GCC_ROOT)$(GCC_LIB)/../../../../arm-none-eabi/lib/thumb/$(CORTEX_VER)/nofp/libm.a
# LIBS += $(GCC_ROOT)$(GCC_LIB)/../../../../arm-none-eabi/lib/thumb/$(CORTEX_VER)/nofp/libstdc++_nano.a

CFLAGS_COMMON += -DUSART_QUEUE_SZ=64 -DHSE_VALUE=8000000U -DCARIBOU_FLOAT=1 -DSTM32L431xx=1
CFLAGS_COMMON += -fshort-enums -fsigned-char -mlittle-endian -mthumb -mthumb-interwork -mcpu=$(MCU_CORE) -mfloat-abi=$(MCU_FLOAT)
CFLAGS_COMMON += -DDEBUG_USART=2 -DRS485_UART=1
CFLAGS_COMMON += -DSERIAL_NO_BASE_ADDRESS=0x1FFFF7AC

CFLAGS_COMMON += -DTIM_OUT_IRQ=TIM1_BRK_UP_TRG_COM_IRQn
CFLAGS_COMMON += -DTIM1_BUS_HZ=48000000				# Timer Reference Frequency
CFLAGS_COMMON += -DTIM2_BUS_HZ=48000000				# Timer Reference Frequency
CFLAGS_COMMON += -DTIM_TRIGGER=TIM15				# Edge-trigger timer (CH1 Only)
CFLAGS_COMMON += -DTIM_OUT=TIM1						# Waveform output timer
# CFLAGS_COMMON += -DTIM_OUT_CHAN=TX_GEN_CHANNEL_1	# Wafeform output channel (TXP)
CFLAGS_COMMON += -DTIM_OUT_CHAN=TX_GEN_CHANNEL_2	# Wafeform output channel (TIM_TEST0)
CFLAGS_COMMON += -DTIM_OUT_BUS_HZ=TIM1_BUS_HZ		# Output waveform timer reference frquency
CFLAGS_COMMON += -DXDUC_HZ=50000					# Transducer frequency
CFLAGS_COMMON += -DXDUC_CYCLES=16					# Number of cycles in a chirp

CFLAGS_COMMON += -DTIM_IN=TIM2						# Waveform output timer
CFLAGS_COMMON += -DTIM_IN_CHAN=RX_GEN_CHANNEL_4		# Waveform output channel
CFLAGS_COMMON += -DTIM_IN_BUS_HZ=TIM2_BUS_HZ		# Output waveform timer reference frquency 

CFLAGS_COMMON += -DPIEZO_MAX_DAC_PWR=0x0AFF
CFLAGS_COMMON += -DPIEZO_TX_XDUC_POWER=50.0f
CFLAGS_COMMON += -DPIEZO_TX_XDUC_HZ=50000			# Transducer frequency
CFLAGS_COMMON += -DPIEZO_TX_XDUC_CYCLES=8			# Number of cycles in a chirp

# sources
include ${SRC_ROOT}/_bsp/${TARGET}/_src.mk
