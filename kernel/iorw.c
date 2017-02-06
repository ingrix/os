#include <kernel/kernel.h>
#include <kernel/iorw.h>

uint8_t ior_byte(uint16_t port) {
  uint8_t b;
  asm("movw %1, %%dx \n\t"
      "inb %%dx \n\t"
      "movb %%al, %0"
      : "=r" (b)
      : "r" (port)
      : "%eax", "%edx");
  return b;
}

void iow_byte(uint16_t port, uint8_t byte) {
  asm("movw %0, %%dx \n\t"
      "movb %1, %%al \n\t"
      "outb %%dx \n\t"
      :
      : "r" (port), "r" (byte)
      : "%eax", "%edx");
}
