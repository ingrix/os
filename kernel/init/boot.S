# 
# This is a multi-boot kernel which is bootable by GRUB
#
# Copyright ingrix 2017
# Code released under the GNU General Public License v. 2.0
# 

#include "setup_asm.h"

.code32

.set ALIGN, 1<<0
.set MEMINFO, 1<<1
.set MB_FLAGS, ALIGN|MEMINFO
.set MB_MAGIC, 0x1BADB002 
.set MB_CHECKSUM, -(MB_MAGIC+MB_FLAGS)

.section .multiboot
.align 4
.long MB_MAGIC
.long MB_FLAGS
.long MB_CHECKSUM

.section .bss
.align 16
stack_bottom:
.skip 16384
stack_top:

.section .text

GLOBAL _istart
# initialization startup
_istart:
  mov $stack_top, %esp
  cli
  
  pushl %ebx # multiboot information struct
  pushl %eax # multiboot magic number, if multiboot

  # check if additional bios functions are needed for basic system
  call check_need_bios
  test %eax, %eax
  jnz cantboot # for now just refuse to boot

  call lm_init # should not return

  cli
  jmp .Lkhang

cantboot:
  # use vga to print off a message since protected mode doesn't have int 0x13
  cld
  movl $VGA_ADDR, %edi
  movl $boot_err_msg, %esi
  lodsb # to %al
.Lmsgloop:
  movb $0x0F, %ah
  stosw
  lodsb
  test %al, %al
  jnz .Lmsgloop

.Lkhang:
  hlt
  jmp .Lkhang

.size _istart, .Lkhang - _istart
.size .Lkhang, . - .Lkhang

# check if any additional bios calls would be needed
# necessary if multiboot wasn't used or 
# hopefully multiboot has given us 
check_need_bios:
  movl 4(%esp), %eax # original eax
  cmpl $MB_MAGIC, %eax
  jne .Lerr

  movl 8(%esp), %edx # multiboot struct address
  movl (%edx), %eax # multiboot flags
  andl $0x20, %eax
  test %eax, %eax
  jz .Lerr

  # check the memory map is valid (size > 0)
  movl 44(%edx), %eax
  test %eax, %eax
  jz .Lerr

  xorl %eax, %eax
  jmp .Lok
  
.Lerr:
  movl $-1, %eax
.Lok:
  ret

.section .rodata
boot_err_msg: .asciz "BIOS calls necessary, unable to boot"

.set VGA_ADDR, 0xB8000
