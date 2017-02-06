#ifndef __K_PAGE_H__
#define __K_PAGE_H__

#include <kernel/kernel.h>

// page frame conversion macros
#define PF_TO_PHYS(x) ((x) << 12)
#define PHYS_TO_PF(x) ((x) >> 12)

struct page {
  uint64_t page_frame;
};

#endif // __K_PAGE_H__
