#ifndef __cplusplus
#include <stdbool.h>
#endif // __cplusplus

#include <stdint.h>
#include <stddef.h>
#include <printk.h>

#ifdef __linux__
#error "Cross compiler is needed to build this kernel"
#endif

#ifndef __i386__
#error "Compiler must be for i686 arch"
#endif

#include <boot.h>
#include <vga.h>
#include <string.h>
#include <multiboot.h>

int kmain(uint32_t magic, uint32_t mboot_info) {
  vga_term_init();

  if(!multiboot_check(magic)) {
    printk("kernel must be booted with multiboot! hanging.");
    khang(); // hang
  }

  puts("booting\n");
  multiboot_init(mboot_info);

  return 0;
}
