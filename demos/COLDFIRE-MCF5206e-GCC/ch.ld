/* STARTUP(crt0.o) */
OUTPUT_ARCH(m68k)
/* OUTPUT_FORMAT(srec)*/
/* OUTPUT_FORMAT(binary) */
/* OUTPUT_FORMAT(ihex) */
/* OUTPUT_FORMAT("elf-m68k") */


__stacks_total_size__	=	8k;

MEMORY
{
	rom (rx)    : ORIGIN = 0x30010000, LENGTH = 448k
	ram (rw)    : ORIGIN = 0x30080000, LENGTH = 512k
/*	eram (rw)    : ORIGIN = 0x00080000, LENGTH = 512k*/
}

__ram_start__		= ORIGIN(ram);
__ram_size__		= LENGTH(ram);
__ram_end__		= __ram_start__ + __ram_size__;


SECTIONS
{
	.text :
	{
		KEEP(*(.start))
		. = ALIGN(0x4);
		*(.text .text.*)
		*(.gnu.linkonce.t.*)
		*(.glue_7t .glue_7)
		KEEP(*(.fini))
		*(.gcc_except_table)
	} > rom
	. = ALIGN(4);

	.ctors :
	{
		PROVIDE(__ctors_start__ = .);
		KEEP(*(SORT(.ctors.*)))
		KEEP(*(.ctors))
		PROVIDE(__ctors_end__ = .);
	} > rom

	.dtors :
	{
		PROVIDE(__dtors_start__ = .);
		KEEP(*(SORT(.dtors.*)))
		KEEP(*(.dtors))
		PROVIDE(__dtors_end__ = .);
	} > rom

	.rodata :
	{
	  	*(.rodata.str1.1)
		*(.rodata .rodata.*)
		*(.gnu.linkonce.r.*)
	} > rom
	. = ALIGN(4);

	_etext = .;
	PROVIDE (etext = .);

	.data : AT(_etext)
	{
		_data = .;
		*(.data)
		*(.shdata)
		. = ALIGN (16);
		*(.ramfunc)
		_edata = .;
		copy_end = .;
	} > ram
	. = ALIGN(4);

	_edata = . ;
	PROVIDE (edata = .);

	.bss :
	{
		__bss_start = .;
		__bss_start__ = .;
		*(.shbss)
		*(.bss)
		*(COMMON)
	} > ram
	. = ALIGN(4);

	__bss_end = . ;
	__bss_end__ = . ;

	PROVIDE (end = .);
	_end = .;

__heap_base__		= _end;
__heap_end__		= __ram_end__ - __stacks_total_size__;

	/* Stabs debugging sections.  */
	.stab          0 : { *(.stab) }
	.stabstr       0 : { *(.stabstr) }
	.stab.excl     0 : { *(.stab.excl) }
	.stab.exclstr  0 : { *(.stab.exclstr) }
	.stab.index    0 : { *(.stab.index) }
	.stab.indexstr 0 : { *(.stab.indexstr) }
	.comment       0 : { *(.comment) }
	/* DWARF debug sections.
     Symbols in the DWARF debugging sections are relative to the beginning
     of the section so we begin them at 0.  */
	/* DWARF 1 */
	.debug          0 : { *(.debug) }
	.line           0 : { *(.line) }
	/* GNU DWARF 1 extensions */
	.debug_srcinfo  0 : { *(.debug_srcinfo) }
	.debug_sfnames  0 : { *(.debug_sfnames) }
	/* DWARF 1.1 and DWARF 2 */
	.debug_aranges  0 : { *(.debug_aranges) }
	.debug_pubnames 0 : { *(.debug_pubnames) }
	/* DWARF 2 */
	.debug_info     0 : { *(.debug_info .gnu.linkonce.wi.*) }
	.debug_abbrev   0 : { *(.debug_abbrev) }
	.debug_line     0 : { *(.debug_line) }
	.debug_frame    0 : { *(.debug_frame) }
	.debug_str      0 : { *(.debug_str) }
	.debug_loc      0 : { *(.debug_loc) }
	.debug_macinfo  0 : { *(.debug_macinfo) }
	/* SGI/MIPS DWARF 2 extensions */
	.debug_weaknames 0 : { *(.debug_weaknames) }
	.debug_funcnames 0 : { *(.debug_funcnames) }
	.debug_typenames 0 : { *(.debug_typenames) }
	.debug_varnames  0 : { *(.debug_varnames) }
/*
	.eram :
	{
		_eram = . ;
	} > eram
*/
}
