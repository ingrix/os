#include <kernel/mm.h>
#include <kernel/printk.h>
#include <kernel/arch/x86/gdt.h>
#include <kernel/string.h>

struct gdt_ptr {
  uint16_t limit;
  struct _gdt_descriptor *base;
} __attribute((packed));

#ifdef __i386__
extern int load_gdt_32(struct gdt_ptr *d);

#define GDT_ENTRIES 6
#define GDT_ENTRY_SIZE 8
#endif // __i386__
static struct _gdt_descriptor gdt_table[GDT_ENTRIES] __attribute__((aligned(PAGE_SIZE))) = { 0 };

static struct _gdt_descriptor *get_gdt_table() {
  return gdt_table;
}

inline uint32_t _gdt_descriptor_get_base(const struct _gdt_descriptor *e) {
  return e ? (((uint32_t)e->base_1) | 
              ((uint32_t)e->base_2 << 16) | ((uint32_t)e->base_3 << 24))
           : 0;
}

inline int _gdt_descriptor_set_base(struct _gdt_descriptor *e, uint32_t base) {
  if(!e) { 
    return -1;
  }
  e->base_1 = (uint16_t)(base & 0xFFFF);
  e->base_2 = (uint8_t)((base >> 16) & 0xFF);
  e->base_3 = (uint8_t)(base >> 24);
  return 0;
}

inline uint32_t _gdt_descriptor_get_limit(const struct _gdt_descriptor *e) {
  return e ? (uint32_t)e->limit_1 | ((uint32_t)e->limit_2 << 16) : 0;
}

inline int _gdt_descriptor_set_limit(struct _gdt_descriptor *e, uint32_t lim) {
  if(!e) {
    return -1;
  }
  e->limit_1 = (uint16_t)(lim & 0xFFFF);
  e->limit_2 = (uint8_t)((lim >> 16) & 0xF);
  return 0;
}

inline uint8_t _gdt_descriptor_get_type(const struct _gdt_descriptor *e) {
  return e ? e->type : 0;
}

inline int _gdt_descriptor_set_type(struct _gdt_descriptor *e, uint8_t type) {
  if(!e) {
    return -1;
  }
  e->type = type;
  return 0;
}

inline uint8_t _gdt_descriptor_get_attr(const struct _gdt_descriptor *e) {
  return e ? e->attr : 0;
}

inline int _gdt_descriptor_set_attr(struct _gdt_descriptor *e, uint8_t attr) {
  if(!e) {
    return -1;
  }
  e->attr = (attr & 0xF);
  return 0;
}

//
// Conversion functions
//

int gdt_entry_to_descriptor(const struct gdt_entry *e, struct _gdt_descriptor *d) {
  if(!e || !d) {
    return -1;
  }
  if(_gdt_descriptor_set_base(d, e->base) != 0) {
    return -1;
  }
  if(_gdt_descriptor_set_limit(d, e->limit) != 0) {
    return -1;
  }
  if(_gdt_descriptor_set_type(d, e->type) != 0) {
    return -1;
  }
  if(_gdt_descriptor_set_attr(d, e->attr) != 0) {
    return -1;
  }
  return 0;
}

int gdt_descriptor_to_entry(const struct _gdt_descriptor *d, struct gdt_entry *e) {
  if(!d || !e) {
    return -1;
  }
  e->base = _gdt_descriptor_get_base(d);
  e->limit = _gdt_descriptor_get_limit(d);
  e->type = _gdt_descriptor_get_type(d);
  e->attr = _gdt_descriptor_get_attr(d);
  return 0;
}

//
// Initialization functions
//

int setup_gdt() {
  struct gdt_entry e = { 0 };
  insert_gdt_entry(&e, __SEG_NULL);

  e.base = 0x0;
  e.limit = ~0;
  e.type = GDT_ENTRY_TYPE_USR | GDT_ENTRY_TYPE_PRES | 0xA;
  e.attr = 0xC; // 4KB granularity, 32-bit operand size
  insert_gdt_entry(&e, __SEG_DEFAULT_KERNEL_CS);

  e.base = 0x0;
  e.limit = ~0;
  e.type = GDT_ENTRY_TYPE_USR | GDT_ENTRY_TYPE_PRES | 0x2;
  e.attr = 0xC; // 4KB granularity, 32-bit stack pointer (if ss)
  insert_gdt_entry(&e, __SEG_DEFAULT_KERNEL_DS);

  e.base = 0x0;
  e.limit = ~0;
  e.type = GDT_ENTRY_TYPE_USR | GDT_ENTRY_TYPE_PRES | 0xA | GDT_ENTRY_DPL(0x3);
  e.attr = 0xC; // 4KB granularity, 32-bit operand size
  insert_gdt_entry(&e, __SEG_DEFAULT_USER_CS);

  e.base = 0x0;
  e.limit = ~0;
  e.type = GDT_ENTRY_TYPE_USR | GDT_ENTRY_TYPE_PRES | 0x2 | GDT_ENTRY_DPL(0x3);
  e.attr = 0xC; // 4KB granularity, 32-bit stack pointer (if ss)
  insert_gdt_entry(&e, __SEG_DEFAULT_USER_DS);

  return 0;
}

void gdt_init() {
  setup_gdt();
  load_gdt();
}

int insert_gdt_entry(const struct gdt_entry *e, unsigned int selector) {
  struct _gdt_descriptor *t;
  if(!e || selector >= 512) {
    return -1;
  }
  t = get_gdt_table();
  return gdt_entry_to_descriptor(e, &t[selector]);
}

int load_gdt() { 
  struct gdt_ptr p;
  p.limit = GDT_ENTRIES * GDT_ENTRY_SIZE;
  p.base = get_gdt_table();
  return load_gdt_32(&p);
}
// int swap_gdt();

