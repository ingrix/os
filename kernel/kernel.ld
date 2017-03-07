OUTPUT_FORMAT(elf64-x86-64)
OUTPUT_ARCH(i386:x86-64)
ENTRY(_start32)

_KTEXT_BASE = 0xffffffff80000000;
_KMAP_BASE = 0xffff800000000000;
_max_stack_len = 0x4000; /* 16kb stack max */

SECTIONS
{
  . = 0x100000;

  _kinit_start = ABSOLUTE(.);
  .multiboot :
  {
    *(.multiboot)
    . = ALIGN(4K);
  }

  .init.text  :
  {
    *(.init.text)
    *(.init.rodata)
    . = ALIGN(4K);
  }

  .init.data  :
  {
    . = ALIGN(4K);
  }

  .init.bss  :
  {
    *(.init.bss)
    . = ALIGN(4K);
  }
  _kinit_end = ABSOLUTE(.);
  _kinit_len = _kinit_end - _kinit_start;

  . += _KTEXT_BASE;

  _kern_start = ABSOLUTE(.);

  _ktext_start = ABSOLUTE(.);
  .text : AT(ADDR(.text) - _KTEXT_BASE)
  {
    *(.text)
    *(.rodata)
  }
  _ktext_end = ABSOLUTE(.);
  _ktext_len = _ktext_end - _ktext_start;

  _kdata_start = ABSOLUTE(.);
  .data : AT(ADDR(.data) - _KTEXT_BASE)
  {
    *(.data)
    . = ALIGN(4K);
  }
  _kdata_end = ABSOLUTE(.);
  _kdata_len = _kdata_end - _kdata_start;

  _kbss_start = ABSOLUTE(.);
  .bss : AT(ADDR(.bss) - _KTEXT_BASE)
  {
    *(COMMON)
    *(.bss)
    . = ALIGN(4K);
  }
  _kbss_end = ABSOLUTE(.);
  _kbss_len = _kbss_end - _kbss_start;

  _kern_end = ABSOLUTE(.);
  _kern_len = _kern_end - _kern_start;

  . += _max_stack_len;
  _init_stack_start = ABSOLUTE(.);
  _heap_start = ABSOLUTE(.);
}
