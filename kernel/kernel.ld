OUTPUT_FORMAT(elf64-x86-64)
OUTPUT_ARCH(i386:x86-64)
ENTRY(_start)

/*
mb_magic  = 0xE85250D6;
mb_arch   = 0x0;
mb_len    = mb_end - mb_start;
mb_chksm  = -(mb_magic + mb_arch + mb_len);
*/
/*
_ktext_start = 0;
_krodata_start = 0;
_krwdata_start = 0;
_kbss_start = 0;
*/
_KTEXT_BASE = 0xffffffff80000000;
_KSPACE_BASE = 0xffff800000000000;

SECTIONS
{
  . = 0x100000;

    /* multiboot header, may set in kernel.ld later */
    /*
    mb_start = .;
    LONG(mb_magic)
    LONG(mb_arch)
    LONG(mb_len) 
    LONG(mb_chksm)
    */

    /* multiboot2 ending tag */
    /*
    SHORT(0)
    SHORT(0)
    LONG(0x08)
    mb_end = .;
    */

  .multiboot :
  {
    *(.multiboot)
    . = ALIGN(4K);
  }

  .init.text  :
  {
    *(.init.text)
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

  . += _KTEXT_BASE;

  _kern_start = .;

  _kro_start = .;
  _ktext_start = .;
  .text : AT(ADDR(.text) - _KTEXT_BASE)
  {
    *(.text)
  }
  _ktext_end = .;

  _krodata_start = .;
  .rodata : AT(ADDR(.rodata) - _KTEXT_BASE)
  {
    *(.rodata)
    . = ALIGN(4K);
  }
  _krodata_end = .;
  _kro_end = .;
  _kro_len = _kro_end - _kro_start;

  _kdata_start = .;
  .data : AT(ADDR(.data) - _KTEXT_BASE)
  {
    *(.data)
    . = ALIGN(4K);
  }
  _kdata_end = .;

  _kbss_start = .;
  .bss : AT(ADDR(.bss) - _KTEXT_BASE)
  {
    *(COMMON)
    *(.bss)
    . = ALIGN(4K);
  }
  _kbss_end = .;

  _ktables_start = .;
  .tables : AT(ADDR(.tables) - _KTEXT_BASE)
  {
    /* kernel pml4 */
    _pml4_0 = .;
    . += 0x1000;

    /* PDPT for low-mapped mem and _KTEXT_BASE */
    _pdpt_0 = .; /* pml4 entry 0 */
    . += 0x1000;
    _pdpt_kern = .; /* pml4 entry 511 */
    . += 0x1000;

    /* page directories for low-mapped and kernel text */
    _pd_0 = .; /* pdpt entry 0 */
    . += 0x1000;
    _pd_kern = .; /* pdpt entry 510, -2GB */
    . += 0x1000;

    /*
    _pt_0 = .;
    . += 0x1000;
    */
  }
  _ktables_end = .;
  _ktables_len = _ktables_end - _ktables_start;
  _kern_end = .;

}
