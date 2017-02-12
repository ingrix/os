ifeq ($(KERN_NAME),)
$(error Cannot build from subdirectory, please run make from parent directory")
endif

SETUP_NAME := setup
SETUP := $(SETUP_NAME).elf
SETUP_BIN := $(SETUP_NAME).bin
LDSCRIPT 			:= $(SETUP_NAME).ld

OBJS := boot.o
OBJS += lm_init.o
#OBJS += page_setup.o
#OBJS += segment_setup.o
#OBJS += map.o
#OBJS += kinit.o

all: $(SETUP)

$(SETUP): $(OBJS)
	$(CC) -T $(LDSCRIPT) $(KERN_CFLAGS) -o $@ $^ $(KERN_LDFLAGS)

%.o: %.c
	$(CC) -c $(KERN_CFLAGS) -o $@ $^ $(KERN_LDFLAGS)

%.o: %.c %.S
	$(CC) -c $(KERN_CFLAGS) -o $@ $^ $(KERN_LDFLAGS)

%.o: %.S
	$(CC) -c $(KERN_CFLAGS) -o $@ $^ $(KERN_LDFLAGS)

.PHONY += clean
clean:
	rm -f $(OBJS) $(SETUP)
