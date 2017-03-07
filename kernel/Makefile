TARGET := x86_64-elf
#TARGET := i686-elf
AS := $(TARGET)-as
LD := $(TARGET)-ld
CC := $(TARGET)-gcc

DEBUG := y

ifneq ($(filter $(D),1 y Y on ON),)
	DEBUGFLAGS := -g
endif
ifneq ($(filter $(DEBUG),1 y Y on ON),)
	DEBUGFLAGS := -g
endif

# standalone arguments
INC					:= -I./include/

SA_CFLAGS 	:= -std=gnu99 -ffreestanding -O2 -Wall -Wextra -nostdlib
SA_CFLAGS   += -mno-red-zone 
SA_CFLAGS   += $(INC) $(DEBUGFLAGS) 

SA_LDFLAGS 	:= -Wl,-z,max-page-size=0x1000 
SA_LDFLAGS  += -lgcc -static-libgcc

# objects to build
KERN_NAME := kernel
KERN 			:= $(KERN_NAME).bin
KERN_SYMS	:= $(KERN_NAME).syms
#
# the order of these is important
#ORDERED_OBJS 	:= boot.o
#C_OBJS				:= $(patsubst %.c,%.o,$(wildcard *.c))
#S_OBJS				:= $(patsubst %.S,%.o,$(wildcard *.S))
#OBJS 					:= $(ORDERED_OBJS) $(filter-out $(ORDERED_OBJS),$(C_OBJS) $(S_OBJS))
EARLY_OBJS		:= boot_setup.o
OBJS					:= boot_32.o $(EARLY_OBJS) kmain.o
LDSCRIPT 			:= $(KERN_NAME).ld

all: kernel

kernel: $(KERN)

$(KERN): $(OBJS) $(LDSCRIPT) 
	$(CC) -T $(LDSCRIPT) $(SA_CFLAGS) -o $@ $^ $(SA_LDFLAGS)

%.o: %.S 
	$(CC) -c $(SA_CFLAGS) -o $@ $< $(SA_LDFLAGS) 
	@#$(AS) -o $@ $<

%.o: %.c include/%.h
	$(CC) -c $(SA_CFLAGS) -o $@ $< 

%.o: %.c 
	@# useful for debugging preprocessor macros
	@#$(CC) -E -dM  - < /dev/null
	$(CC) -c $(SA_CFLAGS) -o $@ $< $(SA_LDFLAGS)

ksyms: $(KERN_SYMS)

$(KERN_SYMS): $(KERN)
	$(TARGET)-objcopy --only-keep-debug $(KERN) $(KERN_SYMS)

$(LDSCRIPT): FORCE

.PHONY: clean

.PHONY += FORCE
clean:
	rm -f *.o $(OBJS) $(KERN)

.PHONY += help
help: 
	@echo -e "64-bit hobby kernel"
	@echo -e "targets:"
	@echo -e "  kernel 	- makes the kernel"
	@echo -e "  ksyms		- makes a debugging output file from kernel"
	@echo -e "  clean		- remove build objects"

FORCE:

