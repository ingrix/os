TARGET := i686-elf
AS := $(TARGET)-as
LD := $(TARGET)-ld
CC := $(TARGET)-gcc
# standalone arguments
INC					:= -I./include/
SA_CFLAGS 	:= -c -std=gnu99 -ffreestanding -O2 -Wall -Wextra $(INC)
SA_LDFLAGS 	:= 

# objects to build
KERN_NAME := barebones
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
	$(LD) -T $(LDSCRIPT) -o $@ $^

%.o: %.S 
	$(AS) -o $@ $<

%.o: %.c include/%.h
	$(CC) $(SA_CFLAGS)$(SA_LDFLAGS) -o $@ $<

%.o: %.c 
	$(CC) $(SA_CFLAGS)$(SA_LDFLAGS) -o $@ $<

.PHONY: clean
clean:
	rm -f *.o $(OBJS) $(KERN)

.PHONY: help
help:
	@echo "barebones kernel v. 0.1
	help!"
