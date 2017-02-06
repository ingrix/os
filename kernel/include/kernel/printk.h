#ifndef __K_PRINTK_H__
#define __K_PRINTK_H__

#include <kernel/kernel.h>

void putchar(int c);
void puts(const char *buf);
void printk(const char *fmt, ...);
void itoa(char *buf, int base, int d); // value to string

#endif // __K_PRINTK_H__
