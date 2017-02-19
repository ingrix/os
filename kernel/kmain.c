#ifndef __cplusplus
#include <stdbool.h>
#endif // __cplusplus

#include <stdint.h>
#include <stddef.h>

#ifdef __linux__
#error "Cross compiler is needed to build this kernel"
#endif

#ifndef __x86_64__
#error "Compiler must be for x86-64 arch"
#endif

int kmain(uint32_t magic, uint32_t mboot_info) {
  asm volatile("movl $42, %%eax\n" : : : "eax");
  return 0;
}
