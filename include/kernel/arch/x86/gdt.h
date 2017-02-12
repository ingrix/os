#ifndef __K_GDT_H__
#define __K_GDT_H__

#ifndef __ASSEMBLER__
#include <kernel.h>

#ifdef __i386__
struct _gdt_descriptor {
  uint16_t  limit_1;        // segment limit bits 0:15
  uint16_t  base_1;         // base address bits 0:15
  uint8_t   base_2;         // base address bits 16:23
  uint8_t   type;           // segment type: DPL, S, P, and type fields
  uint8_t   limit_2:4;      // segment limit
  uint8_t   attr:4;         // seg lim bits 16:19, AVL, op size, granularity
  uint8_t   base_3;         // base address bits 24:31
} __attribute((packed));

struct gdt_entry {
  uint32_t  base;
  uint32_t  limit;
  uint8_t   type;
  uint8_t   attr;
};
#endif // __i386__
#endif // __ASSEMBLER__

//
// Segment type information
//

#define GDT_ENTRY_TYPE_S            4
#define GDT_ENTRY_TYPE_DPL          5
#define GDT_ENTRY_TYPE_P            7

#define GDT_ENTRY_TYPE_USR        (1 << GDT_ENTRY_TYPE_S)
#define GDT_ENTRY_TYPE_PRES       (1 << GDT_ENTRY_TYPE_P)
#define GDT_ENTRY_DPL(x)     (((x) & 0x3) << GDT_ENTRY_TYPE_DPL)

//
// Segment attributes
//

#define GDT_ENTRY_ATTR_AVL          4
#define GDT_ENTRY_ATTR_L            5
#define GDT_ENTRY_ATTR_DB           6
#define GDT_ENTRY_ATTR_GRANULARITY  7

#define GDT_ENTRY_ATTR_AVAIL      (1 << GDT_ENTRY_ATTR_AVL)
#define GDT_ENTRY_ATTR_LONG       (1 << GDT_ENTRY_ATTR_L)
#define GDT_ENTRY_ATTR_
//#define GDT_ENTRY_ATTR_GRAN1B     (0 << GDT_ENTRY_ATTR_GRANULARITY)
#define GDT_ENTRY_ATTR_GRAN4KB    (1 << GDT_ENTRY_ATTR_GRANULARITY)

#define __SEG_NULL              0
#define __SEG_DEFAULT_KERNEL_CS 1
#define __SEG_DEFAULT_KERNEL_DS 2
#define __SEG_DEFAULT_USER_CS   3
#define __SEG_DEFAULT_USER_DS   4

#define __SELECTOR_GDT(x,rpl) (((x) << 3) | ((rpl) & 0x3))
#define __SELECTOR_LDT(x,rpl) (((x) << 3) | ((rpl) & 0x3) | 0x4)

#ifndef __ASSEMBLER__
void gdt_init();
int setup_gdt();
int insert_gdt_entry(const struct gdt_entry *e, unsigned int selector);
int load_gdt();
// int swap_gdt();

#endif // __ASSEMBLER__

#endif // __K_GDT_H__
