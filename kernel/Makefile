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
SA_CFLAGS 	:= -c -std=gnu99 -ffreestanding -O2 -Wall -Wextra $(INC) $(DEBUGFLAGS) 
SA_LDFLAGS 	:= 

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

all: $(KERN) $(SA_FLAGS) $(LD_FLAGS)

$(KERN): $(OBJS)
	$(LD) -T $(LDSCRIPT) -o $@ $^

%.o: %.S 
	$(CC) $(SA_CFLAGS)$(SA_LDFLAGS) -o $@ $<
	@#$(AS) -o $@ $<

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
