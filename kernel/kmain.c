#ifndef __cplusplus
#include <stdbool.h>
#endif // __cplusplus

#include <stdint.h>
#include <stddef.h>
#include <kernel/printk.h>

#ifdef __linux__
#error "Cross compiler is needed to build this kernel"
#endif

#ifndef __i386__
#error "Compiler must be for i686 arch"
#endif

#include <kernel/arch/x86/boot.h>
#include <kernel/arch/x86/gdt.h>
#include <kernel/vga.h>
#include <kernel/string.h>
#include <kernel/multiboot.h>
#include <kernel/mm.h>

int kmain(uint32_t magic, uint32_t mboot_info) {
  vga_term_init();

  if(!multiboot_check(magic)) {
    printk("kernel must be booted with multiboot! hanging.");
    khang(); // hang
  }
  puts("booting\n");

  gdt_init();
  puts("initialized gdt\n");
  multiboot_init(mboot_info);
  mm_init();

  return 0;
}
