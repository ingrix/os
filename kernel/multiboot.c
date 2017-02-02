#include <vga.h>
#include <multiboot.h>

// magic: value in eax passed to the kernel from the bootloader
int multiboot_check(uint32_t magic) {
  return magic == MULTIBOOT_MAGIC;
}

void _print_mb_message(uint32_t mbflags, uint32_t flag, const char *label) {
  if(mbflags & flag) {
    vga_puts_color(" + ", vga_color(VGA_COLOR_GREEN, VGA_COLOR_BLACK));
  } else {
    vga_puts_color(" - ", vga_color(VGA_COLOR_RED, VGA_COLOR_BLACK));
  }
  vga_puts(label);
  vga_putchar('\n');
}

// mboot_info: 32-bit address of where multiboot struct is found in memory
void multiboot_init(uint32_t mboot_info) {
  uint32_t flags = *((uint32_t*)mboot_info);
  _print_mb_message(flags, MB_MEM_FLAG, "hi-low mem");
  _print_mb_message(flags, MB_BOOTDEV_FLAG, "boot device");
  _print_mb_message(flags, MB_CMDLN_FLAG, "command line");
  _print_mb_message(flags, MB_MODS_FLAG, "modules");
  _print_mb_message(flags, MB_SYMS_FLAG, "symbols");
  _print_mb_message(flags, MB_MMAP_FLAG, "memory map");
  _print_mb_message(flags, MB_DRIVES_FLAG, "drives");
  _print_mb_message(flags, MB_CONFTAB_FLAG, "config table");
  _print_mb_message(flags, MB_BOOTNAME_FLAG, "bootloader name");
  _print_mb_message(flags, MB_APMTAB_FLAG, "apm");
  _print_mb_message(flags, MB_VBE_FLAG, "vbe");
  return;
}

