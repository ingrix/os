#include <kernel/vga.h>
#include <kernel/printk.h>
#include <kernel/multiboot.h>
#include <kernel/string.h>

static struct multiboot_info g_mb_info = {0};

// magic: value in eax passed to the kernel from the bootloader
int multiboot_check(uint32_t magic) {
  return magic == MULTIBOOT_MAGIC;
}

int _print_mb_message(uint32_t mbflags, uint32_t flag, const char *label) {
  int r = mbflags & flag;
  if(r) {
    vga_puts_color(" + ", vga_color(VGA_COLOR_GREEN, VGA_COLOR_BLACK));
  } else {
    vga_puts_color(" - ", vga_color(VGA_COLOR_RED, VGA_COLOR_BLACK));
  }
  puts(label);
  putchar('\n');
  return r;
}

// mboot_info: 32-bit address of where multiboot struct is found in memory
void multiboot_init(uint32_t mboot_info) {
  memcpy(&g_mb_info, (struct multiboot_info *)mboot_info, sizeof(g_mb_info));

  if(_print_mb_message(g_mb_info.flags, MB_MEM_FLAG, "hi-low mem")) {
    printk("    lower mem: %dKB, upper mem: %dKB\n", g_mb_info.mem.lower, g_mb_info.mem.upper);
  }
  _print_mb_message(g_mb_info.flags, MB_BOOTDEV_FLAG, "boot device");
  _print_mb_message(g_mb_info.flags, MB_CMDLN_FLAG, "command line");
  _print_mb_message(g_mb_info.flags, MB_MODS_FLAG, "modules");
  _print_mb_message(g_mb_info.flags, MB_SYMS_FLAG, "symbols");
  if(_print_mb_message(g_mb_info.flags, MB_MMAP_FLAG, "memory map")) {
    struct mb_mmap_entry *mm;
    printk("mmap info: %p, 0x%x\n", (void *)g_mb_info.mmap.addr, g_mb_info.mmap.len);
    for(mm = (struct mb_mmap_entry *)g_mb_info.mmap.addr;
        (unsigned long)mm < (g_mb_info.mmap.addr + g_mb_info.mmap.len);
        mm = (struct mb_mmap_entry *)((unsigned long)mm + mm->size + sizeof(mm->size))) {

      printk("   addr: %p, len: 0x%x, type: 0x%x\n", (uint32_t)mm->addr, (uint32_t)mm->len, mm->type);

    }
  }
  _print_mb_message(g_mb_info.flags, MB_DRIVES_FLAG, "drives");
  _print_mb_message(g_mb_info.flags, MB_CONFTAB_FLAG, "config table");
  if(_print_mb_message(g_mb_info.flags, MB_BOOTNAME_FLAG, "bootloader name")) {
    printk("    bootloader: %s\n", (char *)g_mb_info.bootloader_name);
  }
  _print_mb_message(g_mb_info.flags, MB_APMTAB_FLAG, "apm");
  _print_mb_message(g_mb_info.flags, MB_VBE_FLAG, "vbe");

  return;
}
