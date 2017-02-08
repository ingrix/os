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
INC					:= -I./include/
SA_CFLAGS 	:= -std=gnu99 -ffreestanding -O2 -Wall -Wextra -nostdlib $(INC) $(DEBUGFLAGS) 
SA_LDFLAGS 	:= -lgcc -static-libgcc

# objects to build
KERN_NAME := kernel
KERN 			:= $(KERN_NAME).bin
#
# the order of these is important
ORDERED_OBJS 	:= boot.o
C_OBJS				:= $(patsubst %.c,%.o,$(wildcard *.c))
S_OBJS				:= $(patsubst %.S,%.o,$(wildcard *.S))
OBJS 					:= $(ORDERED_OBJS) $(filter-out $(ORDERED_OBJS),$(C_OBJS) $(S_OBJS))
LDSCRIPT 			:= $(KERN_NAME).ld

all: $(KERN)

$(KERN): $(OBJS)
	$(CC) -T $(LDSCRIPT) $(SA_CFLAGS) -o $@ $^ $(SA_LDFLAGS)

%.o: %.S 
	$(CC) -c $(SA_CFLAGS) -o $@ $< $(SA_LDFLAGS) 
	@#$(AS) -o $@ $<

%.o: %.c include/%.h
	$(CC) -c $(SA_CFLAGS) -o $@ $< 

%.o: %.c 
	$(CC) -c $(SA_CFLAGS) -o $@ $< $(SA_LDFLAGS)

.PHONY: clean
clean:
	rm -f *.o $(OBJS) $(KERN)

.PHONY: help
help:
	@echo "barebones kernel v. 0.1
	help!"
