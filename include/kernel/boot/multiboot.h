#ifndef __K_MULTIBOOT_H__
#define __K_MULTIBOOT_H__

#include <kernel.h>

#define MULTIBOOT_MAGIC 0x2BADB002

#define MB_MEM_FLAG      (1 << 0)
#define MB_BOOTDEV_FLAG  (1 << 1)
#define MB_CMDLN_FLAG    (1 << 2)
#define MB_MODS_FLAG     (1 << 3)
#define MB_SYMS_FLAG     (1 << 4 | 1 << 5)
#define MB_MMAP_FLAG     (1 << 6)
#define MB_DRIVES_FLAG   (1 << 7)
#define MB_CONFTAB_FLAG  (1 << 8)
#define MB_BOOTNAME_FLAG (1 << 9)
#define MB_APMTAB_FLAG   (1 << 10)
#define MB_VBE_FLAG      (1 << 11)

/* The symbol table for a.out. */
struct mb_aout_symbol_table
{
  uint32_t tabsize;
  uint32_t strsize;
  uint32_t addr;
  uint32_t reserved;
};

/* The section header table for ELF. */
struct mb_elf_section_header_table
{
  uint32_t num;
  uint32_t size;
  uint32_t addr;
  uint32_t shndx;
};

struct multiboot_info {
  uint32_t flags;

  struct mb_mem {
    uint32_t lower;
    uint32_t upper;
  } mem;

  uint32_t boot_device;
  uint32_t cmdline;

  struct mb_mods {
    uint32_t count;
    uint32_t addr;
  } mods;

  union {
    struct mb_elf_section_header_table elf;
    struct mb_aout_symbol_table aout;
  } syms;

  struct mb_mmap {
    uint32_t len;
    uint32_t addr;
  } mmap;

  struct mb_drives {
    uint32_t len;
    uint32_t addr;
  } drives;

  uint32_t config_table;
  uint32_t bootloader_name;
  uint32_t apm_table;

  struct mb_vbe {
    uint32_t control_info;
    uint32_t mode_info;
    uint32_t mode;
    uint32_t interface_seg;
    uint32_t interface_off;
    uint32_t interface_len;
  } vbe;
}__attribute((packed));

#define MB_MEMORY_AVAIL 1
#define MB_MEMORY_RES 2
struct mb_mmap_entry {
  uint32_t size;
  uint64_t addr;
  uint64_t len;
  uint32_t type;
}__attribute__((packed));


int multiboot_check(uint32_t magic);
void multiboot_init(uint32_t mboot_info);

#endif // __K_MULTIBOOT_H__
