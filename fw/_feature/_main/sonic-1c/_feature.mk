# bsp
include ${FEATURE_ROOT}/caribou/_feature.mk

# sources
include ${SRC_ROOT}/cariboupp/_src.mk
include ${SRC_ROOT}/delay/_src.mk
include ${SRC_ROOT}/util/_src.mk
include ${SRC_ROOT}/led/_src.mk
include ${SRC_ROOT}/xduc-reg-map/_src.mk
include ${SRC_ROOT}/libmodbus/_src.mk
include ${SRC_ROOT}/modbus-slave-register-map/_src.mk
include ${SRC_ROOT}/filesystem/_src.mk
include ${SRC_ROOT}/libs19/_src.mk
include ${SRC_ROOT}/bootstrap/_src.mk
include ${SRC_ROOT}/button/_src.mk
include ${SRC_ROOT}/_bsp/${TARGET}/hw_xduc_adc_dma/_src.mk
include ${SRC_ROOT}/modbus-disco/_src.mk

# source containing main() function
SRCS_CXX += $(FEATURE_ROOT)/${FEATURE}/${TARGET}/main.cpp

SRCS_CC += $(FEATURE_ROOT)/${FEATURE}/${TARGET}/tof-run.c
# SRCS_CC += $(FEATURE_ROOT)/${FEATURE}/${TARGET}/tof-detect.c
SRCS_CC += $(FEATURE_ROOT)/${FEATURE}/${TARGET}/settings.c
SRCS_CC += $(FEATURE_ROOT)/${FEATURE}/${TARGET}/units.c

# final build
include ${FEATURE_ROOT}/_common/build.mk
