# bsp sources
include ${FEATURE_ROOT}/_common/_feature.mk
SRC_BSP=${SRC_ROOT}/_bsp/${TARGET}
INC += -I $(SRC_BSP)

# sources
include ${SRC_ROOT}/libs19/_src.mk
include ${SRC_ROOT}/led/_src.mk
include ${SRC_ROOT}/xprintf/_src.mk

# source containing main() function
SRCS_CC += $(FEATURE_ROOT)/${FEATURE}/main.c

# final build
include ${FEATURE_ROOT}/_common/build.mk
