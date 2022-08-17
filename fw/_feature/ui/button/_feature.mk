# bsp
include ${FEATURE_ROOT}/caribou/_feature.mk

# sources
include ${SRC_ROOT}/delay/_src.mk
include ${SRC_ROOT}/led/_src.mk
include ${SRC_ROOT}/button/_src.mk

# source containing main() function
SRCS_CC += $(FEATURE_ROOT)/${FEATURE}/main.c

# final build
include ${FEATURE_ROOT}/_common/build.mk
