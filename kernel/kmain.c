#ifndef __cplusplus
#include <stdbool.h>
#endif // __cplusplus

#include <stdint.h>
#include <stddef.h>

#ifdef __linux__
#error "Cross compiler is needed to build this kernel"
#endif

#ifndef __i386__
#error "Compiler must be for i686 arch"
#endif

#include <boot.h>
#include <vga.h>

#define MULTIBOOT_MAGIC 0x2BADB002


int kmain(uint32_t magic, uint32_t mboot_info) {
  vga_term_init();
  if(magic != MULTIBOOT_MAGIC) {
    vga_print_msg("kernel must be booted with multiboot! hanging.", 
        vga_color(VGA_COLOR_WHITE, VGA_COLOR_BLACK));
    khang(); // hang
  }
  vga_print_msg("kernel initialized\nbooting", vga_color(VGA_COLOR_WHITE, VGA_COLOR_BLACK));
  return 0;
}
