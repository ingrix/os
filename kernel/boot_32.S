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
.set .L_mb_magic, 0xE85250D6
.set .L_mb_arch, 0x0;
.set .L_mb_len, .L_mb_end - .L_mb_start
.set .L_mb_chksm, -(.L_mb_magic + .L_mb_arch + .L_mb_len);

.L_mb_start:
MB_MAGIC: .long .L_mb_magic
MB_ARCH: .long .L_mb_arch
MB_LEN: .long .L_mb_len
MB_CHKSUM: .long .L_mb_chksm

# empty tag
.short 0
.short 0
.long 0x08
.L_mb_end:

.section .init.data, "aw", @progbits

.Lnolmmsg: .asciz "cannot switch to long mode"

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

.section .init.text, "ax"
.global _start
.type _start, @function

.set _vga_addr, 0xB8000

_start:
  mov $_init_stacktop, %esp
  cli
  
  pushl %ebx # multiboot information struct
  pushl %eax # multiboot magic number, if multiboot

  call clear_vga

  call lm_check
  test %eax, %eax
  jnz .L_nolmerr

  # from here we won't return
  jmp _start64

.L_nolmerr:
  pushl $.Lnolmmsg
  call print_boot_msg

.Lkhang:
  cli
  hlt
  jmp .Lkhang

print_boot_msg:
  movl $_vga_addr, %edi
  movl 4(%esp), %esi
  movb $0x0F, %ah # white foreground, black background
  lodsb
1:
  testb %al, %al
  jz 2f
  stosw
  lodsb
  jmp 1b
2:
  ret

/* int _start64() */
/* return 0 if ok, -1 otherwise */
_start64:

  # disable paging (should be disabled already if multiboot used)
  movl %cr0, %eax
  andl $0x7FFFFFFF, %eax
  movl %eax, %cr0

  call setup_bootstrap_pgtbl
  call enable_lm
  call setup_gdt

  ljmp $gdt.kern_cs64, $lm_activate # transition to 64-bit code

/* int lm_check() */
/* return: 0 if ok, -1 otherwise */
lm_check:
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

/* void setup_bootstrap_pgtbl()
    sets up enough info to get into long mode where the rest of initialization
    can take place */
setup_bootstrap_pgtbl:

  # zero out the page tables
  xor %eax, %eax
  movl $(_ktables_start - _KTEXT_BASE), %edi
  movl $_ktables_len, %ecx
  rep stosb

  # map first 8M of memory into 2MB pages
  movl $(_pd_0 - _KTEXT_BASE), %edi
  xorl %eax, %eax
  orl $0x83, %eax # 2MB page, writable, and present
  movl $4, %ecx # 4 entries, map first 8MB
1:
  stosl
  movl $0, (%edi)
  addl $4, %edi
  addl $0x200000, %eax
  loop 1b

  # set up PDPTEs to point to the PDE we just made
  movl $(_pd_0 - _KTEXT_BASE), %eax
  xorl $0x3, %eax # mark as present and writable

  movl $(_pdpt_0 - _KTEXT_BASE), %edi
  movl %eax, (%edi) # high order bits should be zero

  movl $(_pdpt_0 - _KTEXT_BASE), %eax
  orl $0x3, %eax # writable, present
  movl $(_pml4_0 - _KTEXT_BASE), %edi
  movl %eax, (%edi)

  ret

/* void enable_lm() */
enable_lm:

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

setup_gdt:
  pushl $_init_gdt
  pushw $.L_init_gdt_len
  lgdt (%esp)
  addl $6, %esp
  ret

clear_vga:
  movl $0, %eax
  movl $_vga_addr, %edi
  movl $4000, %ecx
  rep stosl
  ret

.code64

#.extern kmain
lm_activate:

  # activate 64-bit data structures
  mov $gdt.kern_ds64, %ax
  mov %ax, %ds
  mov %ax, %ss
  mov %ax, %es

  # get multiboot information
  movl (%esp), %edi # multiboot magic number
  movl 4(%esp), %esi # multiboot info pointer

  # align the stack to 8-byte boundary
  andq $-16, %rsp
  jmp 1f

  # Should not return
  callq kmain

1:
  cli
  hlt
  jmp 1b
