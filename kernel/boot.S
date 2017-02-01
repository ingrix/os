# 
# This is a multi-boot kernel which is bootable by GRUB
#
# Copyright ingrix 2017
# Code released under the GNU General Public License v. 2.0
# 

.set ALIGN, 1<<0
.set MEMINFO, 1<<1
.set FLAGS, ALIGN|MEMINFO
.set MAGIC, 0x1BADB002 
.set CHECKSUM, -(MAGIC+FLAGS)

.section .multiboot
.align 4
.long MAGIC
.long FLAGS
.long CHECKSUM

.section .bss
.align 16
stack_bottom:
.skip 16384
stack_top:

.section .text
.global _start
.type _start, @function
.global khang
.type khang, @function
_start:
  mov $stack_top, %esp
  cli
  
  pushl %ebx # multiboot information struct
  pushl %eax # multiboot magic number, if multiboot
  call kmain

  cli
khang:
  hlt
  jmp khang

.size _start, khang - _start
.size khang, . - khang
