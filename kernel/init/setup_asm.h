#ifndef __ASSEMBLER__
#error do not include setup_asm.h in non-assembly code
#endif // __ASSEMBLER__

#ifndef __SETUP_ASM_H__
#define __SETUP_ASM_H__

#ifdef __ASSEMBLER__

.macro GLOBAL fxn
.global \fxn
.type \fxn, @function
\fxn:
.endm

#endif // __ASSEMBLER__

#endif // __SETUP_ASM_H__
