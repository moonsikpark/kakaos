OUTPUT_FORMAT(binary)
OUTPUT_ARCH(i386:x86-64)
ENTRY(main)
SECTIONS
{
  PROVIDE (__executable_start = 0x400000); . = 0x400000 + SIZEOF_HEADERS;
  .text 0x200000         :
  {
    *(.text .stub .text.* .gnu.linkonce.t.*)
    /* .gnu.warning sections are handled specially by elf32.em.  */
    *(.gnu.warning)
  } =0x90909090

  .rodata         : { *(.rodata .rodata.* .gnu.linkonce.r.*) }
  . = ALIGN (512);
  
  .data           :
  {
    *(.data .data.* .gnu.linkonce.d.*)
    SORT(CONSTRUCTORS)
  }

  __bss_start = .;
  .bss            :
  {
   *(.dynbss)
   *(.bss .bss.* .gnu.linkonce.b.*)
   *(COMMON)
   . = ALIGN(. != 0 ? 64 / 8 : 1);
  }
}
