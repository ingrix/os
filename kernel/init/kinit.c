#include <kernel.h>

#ifdef __linux__
#error "Cross compiler is needed to build this kernel"
#endif

#ifndef __i386__
#error "Compiler must be for i386 arch"
#endif

int kinit(uint32_t magic, uint32_t mboot_info) {
  return 0;
}
