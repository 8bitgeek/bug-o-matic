# build

OBJS_CXX := $(SRCS_CXX:.cpp=.o)
OBJS_CC  := $(SRCS_CC:.c=.o)
OBJS_AS  := $(SRCS_AS:.S=.o)

${TARGET}: ${TARGET}.elf

${TARGET}.elf: $(OBJS_CXX) $(OBJS_CC) $(OBJS_AS)
	$(LD) $(LFLAGS) -o $@ $^ $(LIBS) 
	$(CP) -O ihex ${TARGET}.elf ${TARGET}.hex
	$(CP) -O srec ${TARGET}.elf ${TARGET}.srec
	$(SZ) ${TARGET}.elf

%.o : %.S %.h
	$(AS) $(ASFLAGS) -o $@  $<

%.o : %.c %.h
	$(CC) $(CFLAGS) -o $@ $<

%.o : %.cpp %.h
	$(CXX) $(CXXFLAGS) -o $@ $<

clean:
		rm -f $(OBJS_CXX) \
				$(OBJS_CC) \
				$(OBJS_AS) \
				${TARGET}.elf \
				${TARGET}.bin \
				${TARGET}.hex \
				${TARGET}.srec \
				${TARGET}.map

flash: ${TARGET}.elf
		$(OC) -f $(OPENOCD_INTERFACE_CFG) \
			-c "$(ENV_COOK_SERIAL)" \
			-c "$(OPENOCD_ADAPTER_SPEED)" \
			-f $(OPENOCD_TARGET_CFG) \
			-c "program ${TARGET}.elf verify reset exit"

openocd:  ${TARGET}.elf
	$(OC) -f $(OPENOCD_INTERFACE_CFG) \
			-c "$(ENV_COOK_SERIAL)" \
			-c "$(OPENOCD_ADAPTER_SPEED)" \
			-f "$(OPENOCD_TARGET_CFG)"

debug: ${TARGET}.elf
	$(OC) -f $(OPENOCD_INTERFACE_CFG) \
			-c "$(ENV_COOK_SERIAL)" \
			-c "$(OPENOCD_ADAPTER_SPEED)"  \
			-f $(OPENOCD_TARGET_CFG) 2> /dev/null &
	$(DB) -quiet $(ENV_COOK_TUI) \
			-x ${COOK_PROJECT}/_gdb/${TARGET}/gdbinit$(ENV_COOK_TUI) \
			${COOK_PROJECT}/${TARGET}.elf

test: ${TARGET}.elf
	$(OC) -f $(OPENOCD_INTERFACE_CFG) \
			-c "$(ENV_COOK_SERIAL)" \
			-c "$(OPENOCD_ADAPTER_SPEED)"  \
			-f $(OPENOCD_TARGET_CFG) 2> /dev/null &
	$(DB)-py -quiet $(ENV_COOK_TUI) \
			-x ${COOK_PROJECT}/_gdb/${TARGET}/gdbinit$(ENV_COOK_TUI) \
			-x ${COOK_PROJECT}/_gdb/_test/${COOK_TEST}.gdb \
			${COOK_PROJECT}/${TARGET}.elf

kill:
	killall openocd
