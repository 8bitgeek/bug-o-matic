CFLAGS_COMMON += -ggdb
CFLAGS_COMMON += -DCOOK_DEBUG=1

# comment these out to use semihosting 
# or implement stdout_putchar to use UART or so
# CFLAGS_COMMON += -Dxstderr=semihost_putchar
# CFLAGS_COMMON += -Dxstdout=semihost_putchar
# CFLAGS_COMMON += -Dxputchar=semihost_putchar
