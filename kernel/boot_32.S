# 
# This is a multi-boot kernel which is bootable by GRUB
#
# Copyright ingrix 2017
# Code released under the GNU General Public License v. 2.0
# 

.set _KTEXT_BASE, 0xffffffff80000000

.code32

.section .multiboot, "a"

# mark these as local so that they don't show up in the damn ELF file
.set mb_magic, 0xE85250D6
.set .L_mb_arch, 0x0;
.set .L_mb_len, .L_mb_end - .L_mb_start
.set .L_mb_chksm, -(mb_magic + .L_mb_arch + .L_mb_len);
.global mb_magic

.L_mb_start:
MB_MAGIC: .long mb_magic
MB_ARCH: .long .L_mb_arch
MB_LEN: .long .L_mb_len
MB_CHKSUM: .long .L_mb_chksm

# alignment tag
.short 0x6 # alignment tag type
.short 0x0 # not optional
.long 0x8 # 8-byte tag.  spec says size=12, may be error?

# empty tag
.short 0
.short 0
.long 0x08
.L_mb_end:

.section .init.rodata, "a", @progbits

.Lnolmmsg: .asciz "cannot switch to long mode\r\n"
.Lnomberrmsg: .asciz "multiboot loader not used, cannot boot\r\n"
.Lmbokmsg: .asciz "multiboot loader was used, booting\r\n"
.Lrebootmsg: .asciz "please reboot...\r\n"

.align 16
_init_gdt:
.quad 0x0 # null descriptor

# kernel code 64-bit descriptor.  We don't actually need most of these fields
.short  0xFFFF  # segment limit 
.short  0x0000  # base 15:00
.byte   0x00    # base 23:16
.byte   0x9a    # P=1, dpl=0, S=1 (non-sys), non-conforming code
.byte   0xAF    # G=4KB, D=0 (necessary), L=1 (64-bit), AVL=0
.byte   0x00    # base 31:24

# kernel data 64-bit descriptor, we only care about present bit and the type
.short  0xFFFF
.short  0x0000
.byte   0x0000
.byte   0x92    # P=1, dpl=0, S=1 (non-sys), writable data
.byte   0xCF    # G=4KB, D=1, L=0 (NA), AVL=0
.byte   0x00

# kernel code 32-bit descriptor, most stuff is important
.short  0xFFFF  # segment limit 
.short  0x0000  # base 15:00
.byte   0x00    # base 23:16
.byte   0x9a    # P=1, dpl=0, S=1 (non-sys), non-conforming code
.byte   0xCF    # G=4KB, D=1 (32-bit)
.byte   0x00    # base 31:24

# kernel data 32-bit descriptor, mostly important
.short  0xFFFF
.short  0x0000
.byte   0x0000
.byte   0x92    # P=1, dpl=0, S=1 (non-sys), writable data
.byte   0xCF    # G=4KB, D=1 (32-bit sp), L=NA, AVL=0
.byte   0x00


_init_gdt_end:

.set .L_init_gdt_len, _init_gdt_end - _init_gdt

.set gdt.kern_cs64, 0x08
.set gdt.kern_ds64, 0x10
.set gdt.kern_cs32, 0x18
.set gdt.kern_ds32, 0x20
.set gdt.kern_tss, 0x30

.section .init.bss, "aw", @nobits
.align 16
_init_stackbottom:
.skip 16384
_init_stacktop:

.global _serialport
_serialport: .skip 2

.global _mb_info_ptr # pointer to multiboot information
_mb_info_ptr: .skip 8

.section .init.text, "ax"
.global _start32
.type _start32, @function

.global COM1
.global COM2
.global COM3
.global COM4

.set COM1, 0x3F8
.set COM2, 0x2F8
.set COM3, 0x3E8
.set COM4, 0x2E8

_start32:
  mov $_init_stacktop, %esp
  mov %esp, %ebp

  cli

  pushl %ebx # multiboot information struct
  pushl %eax # multiboot magic number, if multiboot

  call setup_early_logger_32

  popl %eax
  
  # initial check to ensure multiboot loader was used
  #cmpl $0x36d76289, %eax
  cmpl $0x36d76289, %eax
  jne .L_nomberr

  popl %eax # multiboot 
  movl %eax, _mb_info_ptr

  pushl $.Lmbokmsg
  call write_early_log_32
  addl $4, %esp

  call lm_check_32
  test %eax, %eax
  jnz .L_nolmerr

  # from here we won't return
  jmp enter_long_mode 

.L_nomberr:
  pushl $.Lnomberrmsg
  call write_early_log_32
  addl $4, %esp
  jmp .Lkhang

.L_nolmerr:
  pushl $.Lnolmmsg
  call write_early_log_32
  addl $4, %esp

.Lkhang:
  pushl $.Lrebootmsg
  call write_early_log_32
  addl $4, %esp
1:
  cli
  hlt
  jmp 1b

setup_early_logger_32:

  # maybe this is a dumb way to do this?
  movw $COM1, %dx
  movw %dx, _serialport
  addw $0x7, %dx # scratch register
  call .Ltestcom
  test %eax, %eax
  jz 1f

  movw $COM2, %dx
  movw %dx, _serialport
  addw $0x7, %dx # scratch register
  call .Ltestcom
  test %eax, %eax
  jz 1f

  movw $COM3, %dx
  movw %dx, _serialport
  addw $0x7, %dx # scratch register
  call .Ltestcom
  test %eax, %eax
  jz 1f

  movw $COM4, %dx
  movw %dx, _serialport
  addw $0x7, %dx # scratch register
  call .Ltestcom
  test %eax, %eax
  jnz 2f

1:
  movw _serialport, %dx
  
  # set the DLAB to 0 to make sure we write data
  addw $3, %dx
  inb %dx
  andb $0x7F, %al
  outb %dx

  ret
2:
  movw $0, _serialport
  ret

.Ltestcom:
  movb $0x42, %al # arbitrary value
  outb %dx
  xorb %al, %al
  inb %dx
  cmpb $0x42, %al
  je 1f
  movl $-1, %eax
  ret
1:
  xorl %eax, %eax
  ret
  

write_early_log_32:
  movw _serialport, %dx
  testw %dx, %dx
  jz 2f

  movl 4(%esp), %esi
1:
  lodsb
  testb %al, %al
  jz 2f
  outb %dx
  jmp 1b
2:
  ret

/* int enter_long_mode() */
/* return 0 if ok, -1 otherwise */
enter_long_mode:

  # disable paging (should be disabled already if multiboot used)
  movl %cr0, %eax
  andl $0x7FFFFFFF, %eax
  movl %eax, %cr0

  call setup_bootstrap_pgtbl_32
  call enable_lm_32
  call setup_gdt_32

  ljmp $gdt.kern_cs64, $.Llm_activate # transition to 64-bit code

/* int lm_check_32() */
/* return: 0 if ok, -1 otherwise */
lm_check_32:
  # let's save ebx
  pushl %ebx

  # check for cpuid first
  pushf # orig eflags
  pushf # test value
  xorl $0x200000, (%esp) # bit 21 is CPUID support
  popf # should flip CPUID bit
  pushf # store again
  popl %eax
  xorl (%esp), %eax # get changed bits
  popf # restore eflags
  andl $0x200000, %eax # changed bits includes bit 29?
  jz .L_lm_check_fail # CPUID not supported

  # check for extended cpuid functions
  movl $0x80000000, %eax
  cpuid
  cmpl $0x80000001, %eax # check for extended features
  jb .L_lm_check_fail

  # check for long mode
  movl $0x80000001, %eax
  cpuid
  andl $0x20000000, %edx # test bit 29, if set then lm available
  jz .L_lm_check_fail

  xorl %eax, %eax
  jmp .L_lm_check_end
.L_lm_check_fail:
  movl $-1, %eax
.L_lm_check_end:
  popl %ebx
  ret

/* void setup_bootstrap_pgtbl_32()
    sets up enough info to get into long mode where the rest of initialization
    can take place */
setup_bootstrap_pgtbl_32:

  # zero out the page tables
  xor %eax, %eax
  movl $(_ktables_start - _KTEXT_BASE), %edi
  movl $_ktables_len, %ecx
  rep stosb

  # determine number of 2M pages we need to cover the whole kernel space
  movl $(_kern_end - _KTEXT_BASE), %ecx
  addl $_max_init_heap, %ecx
  addl $0x200000, %ecx
  shr $21, %ecx
  movl %ecx, %edx # save for later

  # map kernel text to the first entries of _pd_ktext
  movl $(_pd_ktext - _KTEXT_BASE), %edi
  xorl %eax, %eax
  orl $0x81, %eax # 2MB page, read-only, present
1:
  stosl
  movl $0, (%edi)
  addl $4, %edi
  addl $0x200000, %eax
  loop 1b

  # also identity map the memory up to the end of the kernel
  movl $(_pd_0 - _KTEXT_BASE), %edi
  xorl %eax, %eax
  orl $0x83, %eax # 2MB page, writable, and present
  movl %edx, %ecx
1:
  stosl
  movl $0, (%edi)
  addl $4, %edi
  addl $0x200000, %eax
  loop 1b

  # put ktext entry in pdpt
  movl $(_pd_ktext - _KTEXT_BASE), %eax
  orl $0x1, %eax # present, read-only

  # place the identity-mapped memory in the pdpt
  movl $(_pd_0 - _KTEXT_BASE), %eax
  orl $0x3, %eax 
  movl $(_pdpt_0 - _KTEXT_BASE), %edi
  movl %eax, (%edi) # high order bits should be zero

  # identity-mapped pml4
  movl $(_pdpt_0 - _KTEXT_BASE), %eax
  orl $0x3, %eax
  movl $(_pml4_0 - _KTEXT_BASE), %edi
  movl %eax, (%edi)

  ret

/* void enable_lm_32() */
enable_lm_32:

  # set CR0.PAE=1, 
  # setup CR3 with pml4
  # set EFER.LME=1 in MSR 0xC0000080

  # make pml4 available
  movl $(_pml4_0 - _KTEXT_BASE), %eax
  movl %eax, %cr3

  # enable PAE and PSE
  movl %cr4, %eax
  orl $0x30, %eax
  movl %eax, %cr4

  .set EFER, 0xC0000080
  .set EFER.SCE, 0x001
  .set EFER.LME, 0x100
  .set EFER.NXE, 0x800

  # set long-mode enable in EFER
  movl $EFER, %ecx
  rdmsr
  orl $(EFER.SCE | EFER.LME | EFER.NXE), %eax
  wrmsr

  movl %cr0, %eax
  orl $0x80000020, %eax # enable paging and pae
  movl %eax, %cr0

  # enable global pages now that paging is enabled
  movl %cr4, %eax
  orl $0x80, %eax
  movl %eax, %cr4

  ret

setup_gdt_32:
  pushl $_init_gdt
  pushw $.L_init_gdt_len
  lgdt (%esp)
  addl $6, %esp
  ret

.code64

#.extern kmain
.Llm_activate:

  # activate 64-bit data structures
  mov $gdt.kern_ds64, %ax
  mov %ax, %ds
  mov %ax, %ss
  mov %ax, %es

  # get mb magic (eax) and mb structure address (ebx)
  xor %rdi, %rdi
  movq %rdi, %rsi
  movl (%esp), %edi # we will use sys V abi here
  movl 4(%esp), %esi

  # align the stack to 8-byte boundary
  andq $-16, %rsp

  jmp setup_kern

_map_ktext:
  /* add a pdpt and pd for higher-half kernel text/data */
  movq $_kern_len, %rcx
  ret
