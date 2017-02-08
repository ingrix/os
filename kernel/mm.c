#include <kernel/mm.h>
#include <kernel/printk.h>

extern void *_kern_end;

static void *heap_start;
static void *heap_end;

int mm_init() {
  printk("kern_end is at %p\n", _kern_end);
  heap_start  = _kern_end;
  heap_end    = _kern_end + PAGE_SIZE;
  return 0;
}
