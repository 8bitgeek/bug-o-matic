# bsp
include ${FEATURE_ROOT}/caribou/_feature.mk

# sources
include ${SRC_ROOT}/syslog/_src.mk
include ${SRC_ROOT}/freemodbus/_src.mk
include ${SRC_ROOT}/delay/_src.mk
include ${SRC_ROOT}/util/_src.mk
include ${SRC_ROOT}/assert/_src.mk
include ${SRC_ROOT}/modbus-slave-register-map/_src.mk
include ${SRC_ROOT}/xduc-state-machine/_src.mk
include ${SRC_ROOT}/piezo/tx/_src.mk
include ${SRC_ROOT}/xduc-reg-map/_src.mk
include ${SRC_ROOT}/led/_src.mk
include ${SRC_ROOT}/rx-gain/_src.mk
include ${SRC_ROOT}/s19/_src.mk

# bsp sources
include $(SRC_BSP)/modbus/port/_src.mk

# source containing main() function
SRCS_CC += $(FEATURE_ROOT)/${FEATURE}/main.c

# final build
include ${FEATURE_ROOT}/_common/build.mk
