.macro define_global function
.global \function
.type \function, @function
.endm

.section .text
define_global load_pgdir
define_global enable_paging
define_global disable_paging

# args: 4(%esp) = page directory physical address
load_pgdir :
  movl 4(%esp), %eax
  movl %eax, %cr3
  ret

enable_paging:
  movl %cr0, %eax
  orl $0x80000000, %eax
  movl %eax, %cr0
  ret

disable_paging:
  movl %cr0, %eax
  andl $0x0FFFFFF, %eax
  movl %eax, %cr0
  ret

.section .pgtbl, "aw", @progbits
.global _init_pgdir
.global _init_pgtbl

.align 0x1000
_init_pgdir:
. = . + 0x1000
_init_pgtbl:
. = . + 0x1000
