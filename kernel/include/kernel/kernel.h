#ifndef __K_KERNEL_H__
#define __K_KERNEL_H__

#include <stddef.h>
#include <stdint.h>

#define KERN_NAME "kernel"
#define KERN_MAJOR "0"
#define KERNEL_MINOR "1"

#define MAX(x,y) ((x) > (y) ? (x) : (y))
#define MIN(x,y) ((x) < (y) ? (x) : (y))

#define PAGE_SIZE 4096

#endif // __K_KERNEL_H__
