include ../../../../config.mk

###################################################

vpath %.c src

CFLAGS += -ffreestanding -nostdlib

# Includes
CFLAGS += -Iinc -I../../../Core/cmsis -I../../../Core/stm32
CFLAGS += -I../../../Conf -I../../../USB_OTG/inc
CFLAGS += -I../../Core/inc

# Sources
SRCS = usbh_cdc_core.c

OBJS = $(SRCS:.c=.o)
LIBNAME = libusbhostcdc.a

all: $(LIBNAME)

%.o : %.c
	$(CC) $(CFLAGS) -c -o $@ $^

$(LIBNAME): $(OBJS)
	$(AR) -r $@ $(OBJS)

clean:
	rm -f $(OBJS) $(LIBNAME)
	
