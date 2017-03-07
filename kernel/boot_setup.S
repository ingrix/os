
.extern _serialport
.extern _mb_info_ptr

.section .init.bss, "aw", @nobits

.global _mb_mmap_info_ptr
.global _mb_lowmem_info_ptr
_mb_mmap_info_ptr: .skip 8
_mb_lowmem_info_ptr: .skip 8

.section .text, "ax"

# void setup_kern(uint32_t mb_magic, uint32_t mb_info_phys_addr)
# %rdi should contain the mb magic number and %rsi should have the multiboot
# information physical address
# stack will be 16-byte aligned on entry to this function
.global setup_kern
setup_kern:
  cmpq $0x800000, %rsi
  jge .Lmbphyserr

  subq $0x10, %rsp
  movq %rsi, (%rsp)

  movq $.Lokmsg, %rdi
  callq write_early_log

  movq (%rsp), %rdi
  callq _find_mb_mem_info 

.Lhlt:
  cli
  hlt
  jmp .Lhlt

.Lmbphyserr:
  movq $.Lmberr, %rdi
  callq write_early_log
  jmp .Lhlt

/* int _find_mb_mem_info()
  returns flags of what memory info was found */
.set MB_MMAP, 0x2
.set MB_LOWMEM, 0x1
_find_mb_mem_info:
  pushq $0
  movq $0, _mb_lowmem_info_ptr
  movq $0, _mb_mmap_info_ptr

  movq _mb_info_ptr, %rsi
  movq (%rsi), %rcx # total size of multiboot header
  cmpq $0x8, %rcx
  jle .Le

  leaq 8(%rsi), %rsi # skip initial header
1:
  movl (%rsi), %r11d
  movl 4(%rsi), %eax

  leaq 7(%rax), %rcx
  andq $-8, %rcx # 8-byte-padded size of tag

  testq %r11, %r11 # ending tag
  jz .Le 
  cmpq $0x6, %r11 # memory tag
  je .Lmmapinfo
  cmpq $0x4, %r11
  je .Lmeminfo

2:
  leaq (%rsi,%rcx), %rsi
  jmp 1b
.Le:
  popq %rax
  ret

.Lmmapinfo:
  movq %rsi, _mb_mmap_info_ptr
  orq $MB_MMAP, (%rsp)
  jmp 2b

.Lmeminfo:
  movq %rsi, _mb_lowmem_info_ptr
  orq $MB_LOWMEM, (%rsp)
  jmp 2b

.global write_early_log
.type write_early_log, @function

/* void write_early_log(const char *) */
write_early_log:
  movq %rdi, %rsi
  movw _serialport, %dx
  testw %dx, %dx
  jz 2f
1:
  lodsb
  testb %al, %al
  jz 2f
  outb %dx
  jmp 1b
2:
  ret

write_early_log_char:
  movw %di, %ax
  testb %al, %al
  jz 1f
  movw _serialport, %dx
  testw %dx, %dx
  jz 1f
  outb %dx
1:
  ret

.section .init.rodata, "a"
.Lokmsg: .asciz "booting\r\n"
.Lmberr: .asciz "multiboot struct out of range\r\n"
.Lmb_info_hdr: .asciz "multiboot information\r\n"
.Lmb_tag_size: .asciz "size: "
.Lmb_tag_type: .asciz "type: "
.Lnewline: .asciz "\r\n"
