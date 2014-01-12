/*
    ChibiOS/RT - Copyright (C) 2006,2007,2008,2009,2010,
                 2011,2012,2013,2014 Giovanni Di Sirio.

    This file is part of ChibiOS/RT.

    ChibiOS/RT is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 3 of the License, or
    (at your option) any later version.

    ChibiOS/RT is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

/*
 * R5F100xE memory setup.
 */
__main_stack_size__     = 0x100;

MEMORY
{
    vectors : org = 0x0,     len = 128
    option  : org = 0xC0,    len = 4
    flash :   org = 0x100,   len = 64k-0x100
    ram :     org = 0xFEF00, len = 4k-32
}

__ram_start__           = ORIGIN(ram);
__ram_size__            = LENGTH(ram);
__ram_end__             = __ram_start__ + __ram_size__;
__flash_start__         = ORIGIN(flash);
__flash_size__          = LENGTH(flash);

SECTIONS
{
    . = 0;
    _text = .;

    .vectors : ALIGN(4)
    {
        KEEP(*(.vectors))
    } > vectors

	.option : ALIGN(4)
	{
          KEEP(*(.option))
	} > option

    .text : ALIGN(2) SUBALIGN(2)
    {
        *(.text P .stub .text.* .gnu.linkonce.t.*)
        KEEP (*(.text.*personality*))
        /* .gnu.warning sections are handled specially by elf32.em.  */
        *(.gnu.warning)
        *(.interp .hash .dynsym .dynstr .gnu.version*)
	} > flash

    /* mirrored code area in data space */
	.rodata (MAX(__romdatastart + __romdatacopysize, 0x2000)) : {
     . = ALIGN(2);
     *(.plt)
     *(.rodata C C_2 C_1 .rodata.* .gnu.linkonce.r.*)
     *(.rodata1)
     *(.eh_frame_hdr)
     KEEP (*(.eh_frame))
     KEEP (*(.gcc_except_table)) *(.gcc_except_table.*)
     PROVIDE (__preinit_array_start = .);
     KEEP (*(.preinit_array))
     PROVIDE (__preinit_array_end = .);
     PROVIDE (__init_array_start = .);
     KEEP (*(SORT(.init_array.*)))
     KEEP (*(.init_array))
     PROVIDE (__init_array_end = .);
     PROVIDE (__fini_array_start = .);
     KEEP (*(.fini_array))
     KEEP (*(SORT(.fini_array.*)))
     PROVIDE (__fini_array_end = .);
     LONG(0); /* Sentinel.  */

     /* gcc uses crtbegin.o to find the start of the constructors, so
        we make sure it is first.  Because this is a wildcard, it
        doesn't matter if the user does not actually link against
        crtbegin.o; the linker won't look for a file to match a
        wildcard.  The wildcard also means that it doesn't matter which
        directory crtbegin.o is in.  */
     KEEP (*crtbegin*.o(.ctors))

     /* We don't want to include the .ctor section from from the
        crtend.o file until after the sorted ctors.  The .ctor section
        from the crtend file contains the end of ctors marker and it
        must be last */
     KEEP (*(EXCLUDE_FILE (*crtend*.o ) .ctors))
     KEEP (*(SORT(.ctors.*)))
     KEEP (*(.ctors))

     KEEP (*crtbegin*.o(.dtors))
     KEEP (*(EXCLUDE_FILE (*crtend*.o ) .dtors))
     KEEP (*(SORT(.dtors.*)))
     KEEP (*(.dtors))
   } > flash
/*
    .textalign : ONLY_IF_RO
    {
        . = ALIGN(8);
    } > flash
*/
    _etext = .;
    _textdata = _etext;

    .noinit (NOLOAD) :
    {
    } > ram

    .stacks :
    {
        . = ALIGN(2);
        __main_thread_stack_base__ = .;
        . += __main_stack_size__;
        . = ALIGN(2);
        __main_thread_stack_end__ = .;
    } > ram

    .data :
    {
        . = ALIGN(2);
        PROVIDE(_data = .);

        KEEP (*(.jcr))
        *(.data.rel.ro.local) *(.data.rel.ro*)
        *(.dynamic)

        *(.data D .data.* .gnu.linkonce.d.*)
        KEEP (*(.gnu.linkonce.d.*personality*))
        SORT(CONSTRUCTORS)
        *(.data1)
        *(.got.plt) *(.got)

        /* We want the small data sections together, so single-instruction offsets
           can access them all, and initialized data all before uninitialized, so
           we can shorten the on-disk segment size.  */
        . = ALIGN(2);
        *(.sdata .sdata.* .gnu.linkonce.s.* D_2 D_1)

        . = ALIGN(2);

        PROVIDE(_edata = .);
    } > ram AT > flash

    PROVIDE(__romdatastart = LOADADDR(.data));
    PROVIDE (__romdatacopysize = SIZEOF(.data));

	.bss :
    {
        . = ALIGN(2);
        PROVIDE(_bss_start = .);
        *(.dynbss)
        *(.sbss .sbss.*)
        *(.bss B B_2 B_1 .bss.* .gnu.linkonce.b.*)
        . = ALIGN(2);
        *(COMMON)
        . = ALIGN(2);
        PROVIDE(_bss_end = .);
    } > ram
}

PROVIDE(end = .);
_end            = .;

__heap_base__   = _end;
__heap_end__    = __ram_end__;
