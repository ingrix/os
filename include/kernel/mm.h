#ifndef __K_MM_H__
#define __K_MM_H__

#include <kernel.h>

#define PAGE_SIZE 4096

// page frame conversion macros
#define PF_TO_PHYS(x) ((x) << 12)
#define PHYS_TO_PF(x) ((x) >> 12)

void init_pagetable();

void enable_paging();
void disable_paging();



#endif // __K_MM_H__
