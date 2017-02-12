#include "setup_asm.h"

.section .text

GLOBAL lm_init
  ret

.section .data
.macro PGD_ENTRY base, limit, type, attr
.word (\limit & 0xFFFF)
.word (\base & 0xFFFF)
.byte ((\base >> 16) & 0xFF) 
.byte \type
.byte (\attr << 4) | ((\limit >> 16) & 0xF)
.byte (\base >> 24)
.endm

_tmp_pgdir:
PGD_ENTRY 0x0, 0x0, 0x0, 0x0 # null segment
PGD_ENTRY 0x0, 0xFFFFF, 0x9a, 0xE # kernel code, 64-bit
PGD_ENTRY 0x0, 0xFFFFF, 0x92, 0xE # kernel data, 64-bit
_tmp_pgtbl:
