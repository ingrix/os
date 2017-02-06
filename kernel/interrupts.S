
.section .text
.global disable_interrupts
.type disable_interrupts, @function
.global enable_interrupts
.type enable_interrupts, @function

disable_interrupts:
  cli
  ret

enable_interrupts:
  sti
  ret
