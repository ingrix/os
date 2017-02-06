#include <kernel/arch/x86/gdt.h>

.section .text
.global load_gdt_32
.type load_gdt_32, @function

load_gdt_32:
  movl 4(%esp), %eax
  lgdt (%eax)

  mov $__SELECTOR_GDT(__SEG_DEFAULT_KERNEL_DS,0), %ax
  mov %ax, %ds
  mov %ax, %ss
  mov %ax, %es
  mov %ax, %gs
  mov %ax, %fs

  ljmp $__SELECTOR_GDT(__SEG_DEFAULT_KERNEL_CS, 0), $.Lflush
.Lflush:
  xorl %eax, %eax
  ret

