
srcroot := $(PWD)
subdirs=kernel

export srcroot

.PHONY: all
all: kernel

.PHONY: kernel
kernel:
	$(MAKE) -C kernel

kernel-clean:
	$(MAKE) -C kernel clean

.PHONY: clean
clean: kernel-clean
