#ifndef __K_MM_H__
#define __K_MM_H__

#include <kernel/kernel.h>

#define PAGE_SIZE 4096

struct page {
#if defined(__x86_64__)
  uint64_t page_frame;
#elif defined(__i386__) // end __x86_64__
  uint32_t page_frame;
#endif // __i386__
};

int mm_init();

#endif // __K_MM_H__
