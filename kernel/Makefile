srcroot ?= $(dir $(PWD))
include $(srcroot)/config.mk

TARGET := i686-elf
AS := $(TARGET)-as
LD := $(TARGET)-ld
CC := $(TARGET)-gcc

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

export KERN_CFLAGS KERN_LDFLAGS

# objects to build
KERN_NAME := kernel
KERN 			:= $(KERN_NAME).elf
KERN_SYMS := $(KERN_NAME).syms

export KERN_NAME

#
# the order of these is important
ORDERED_OBJS 	:= boot.o
C_OBJS				:= $(patsubst %.c,%.o,$(wildcard *.c))
S_OBJS				:= $(patsubst %.S,%.o,$(wildcard *.S))
OBJS 					:= $(ORDERED_OBJS) $(filter-out $(ORDERED_OBJS),$(C_OBJS) $(S_OBJS))
LDSCRIPT 			:= $(KERN_NAME).ld

subtargets := boot

all: $(KERN) FORCE

$(KERN): $(targets) $(OBJS)
	$(CC) -T $(LDSCRIPT) $(KERN_CFLAGS) -o $@ $^ $(KERN_LDFLAGS)

%.o: %.S 
	$(CC) -c $(KERN_CFLAGS) -o $@ $< $(KERN_LDFLAGS) 
	@#$(AS) -o $@ $<

%.o: %.c include/%.h
	$(CC) -c $(KERN_CFLAGS) -o $@ $< 

%.o: %.c 
	$(CC) -c $(KERN_CFLAGS) -o $@ $< $(KERN_LDFLAGS)

ksyms: $(KERN_SYMS)

$(KERN_SYMS): $(KERN)
	objcopy --only-keep-debug $(KERN) $(KERN_SYMS)

$(subtargets):
	$(MAKE) $(build_dir)=$@

.PHONY: clean
clean:
	rm -f *.o $(OBJS) $(KERN) $(KERN_SYMS)

.PHONY: help
help:
	@echo "kernel v. 0.1
	help!"
