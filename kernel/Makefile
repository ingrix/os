srcroot ?= $(dir $(PWD))
include $(srcroot)/config.mk

TARGET := x86_64-elf
AS := $(TARGET)-as
LD := $(TARGET)-ld
CC := $(TARGET)-gcc

export AS LD CC

ifneq ($(filter $(D),1 y Y on ON),)
	DEBUGFLAGS := -g
endif
ifneq ($(filter $(DEBUG),1 y Y on ON),)
	DEBUGFLAGS := -g
endif

# standalone arguments
KERN_INCLUDES := -I$(srcroot)/include/kernel/ -I$(PWD)/include/
KERN_CFLAGS 	:= -std=gnu99 -ffreestanding -O2 -Wall -Wextra -nostdlib $(KERN_INCLUDES) $(DEBUGFLAGS) 
KERN_LDFLAGS 	:= -lgcc -static-libgcc

export KERN_CFLAGS KERN_LDFLAGS KERN_INCLUDES

# objects to build
KERN_NAME := kernel
KERN 			:= $(KERN_NAME).elf
KERN_SYMS := $(KERN_NAME).syms

export KERN_NAME

#
# the order of these is important
#ORDERED_OBJS 	:= boot.o
ORDERED_OBJS 	:= 
#C_OBJS				:= $(patsubst %.c,%.o,$(wildcard *.c))
#S_OBJS				:= $(patsubst %.S,%.o,$(wildcard *.S))
#KERN_OBJS 					:= $(ORDERED_OBJS) $(filter-out $(ORDERED_OBJS),$(C_OBJS) $(S_OBJS))
KERN_OBJS					:= test.o
LDSCRIPT 			:= $(KERN_NAME).ld

KERN_DEPS :=	init 

all: kernel FORCE

kernel: $(KERN_DEPS) $(KERN)

$(KERN): $(KERN_OBJS)
	$(CC) -T $(LDSCRIPT) $(KERN_CFLAGS) -o $@ $^ $(KERN_LDFLAGS)

%.o: %.S 
	$(CC) -c $(KERN_CFLAGS) -o $@ $< $(KERN_LDFLAGS) 
	@#$(AS) -o $@ $<

%.o: %.c 
	$(CC) -c $(KERN_CFLAGS) -o $@ $< $(KERN_LDFLAGS)

%.o: %.c %.S
	$(CC) -c $(KERN_CFLAGS) -o $@ $^ $(KERN_LDFLAGS)

ksyms: $(KERN_SYMS)

$(KERN_SYMS): $(KERN)
	objcopy --only-keep-debug $(KERN) $(KERN_SYMS)

$(KERN_DEPS): FORCE
	@$(MAKE) -C $@

FORCE:

.PHONY: clean
clean:
	@for i in $(KERN_DEPS) ; do $(MAKE) -C $$i clean ; done
	rm -f *.o $(KERN_OBJS) $(KERN) $(KERN_SYMS)

.PHONY: help
help:
	@echo "kernel v. 0.1"
	@echo "  targets: all ksyms clean $(KERN_DEPS)"
