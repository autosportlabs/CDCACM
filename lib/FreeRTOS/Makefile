include ../../config.mk

#If no heap implementation has been defined, choose heap 1
# Note that this implementation does not allow free
ifeq ($(FREERTOS_HEAP),)
$(warning No heap selected, defaulting to heap 1 (Memory cannot be freed!!))
FREERTOS_HEAP = heap_1
endif

vpath %.c Source
vpath %.c Source/portable/GCC/ARM_CM4F
vpath %.c Source/portable/MemMang

CFLAGS += -ffreestanding -nostdlib

# Includes
CFLAGS += -I../Core/cmsis -I../Core/stm32
CFLAGS += -I../Conf -ISource/include -ISource/portable/GCC/ARM_CM4F

# Sources
SRCS = croutine.c list.c queue.c tasks.c timers.c
SRCS += port.c
SRCS += $(FREERTOS_HEAP).c

OBJS = $(SRCS:.c=.o)
LIBNAME = libfreertos.a

all: $(LIBNAME)

%.o : %.c
	$(CC) $(CFLAGS) -c -o $@ $^
%.o : %.s
	$(CC) $(ASFLAGS) -c -o $@ $^

$(LIBNAME): $(OBJS)
	$(AR) -r $@ $(OBJS)

clean:
	rm -f $(OBJS) $(LIBNAME)
	
