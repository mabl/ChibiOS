[#ftl]
[@pp.dropOutputFile /]
[@pp.changeOutputFile name="application.ld" /]
/*
    ChibiOS/RT - Copyright (C) 2006,2007,2008,2009,2010,
                 2011,2012 Giovanni Di Sirio.

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
 * ST32F4xx memory setup.
 */
__main_stack_size__     = ${conf.groups.runtime_settings.irq_stack_size[0]};
__process_stack_size__  = ${conf.groups.runtime_settings.c_stack_size[0]};

MEMORY
{
[#if conf.groups.platform_settings.specific_model[0] == "STM32F4xxyE"]
    flash : org = 0x08000000, len = 512k
[#if conf.groups.memory_layout_settings.ram_areas_organization[0].@index[0]?string == "0"]
    ram : org = 0x20000000, len = 128k
[#else]
    ram : org = 0x20000000, len = 112k
    ethram : org = 0x2001C000, len = 16k
[/#if]
    ccmram : org = 0x10000000, len = 64k

[#elseif conf.groups.platform_settings.specific_model[0] == "STM32F4xxyG"]
    flash : org = 0x08000000, len = 1M
[#if conf.groups.memory_layout_settings.ram_areas_organization[0].@index[0]?string == "0"]
    ram : org = 0x20000000, len = 128k
[#else]
    ram : org = 0x20000000, len = 112k
    ethram : org = 0x2001C000, len = 16k
[/#if]
    ccmram : org = 0x10000000, len = 64k

[#elseif conf.groups.platform_settings.specific_model[0] == "STM32F4xxyI"]
    flash : org = 0x08000000, len = 2M
[#if conf.groups.memory_layout_settings.ram_areas_organization[0].@index[0]?string == "0"]
    ram : org = 0x20000000, len = 128k
[#else]
    ram : org = 0x20000000, len = 112k
    ethram : org = 0x2001C000, len = 16k
[/#if]
    ram2 : org = 0x20020000, len = 64k
    ccmram : org = 0x10000000, len = 64k

[#else]
[#stop "unknown ST32F4xx model"]
[/#if]
}

__ram_start__           = ORIGIN(ram);
__ram_size__            = LENGTH(ram);
__ram_end__             = __ram_start__ + __ram_size__;

SECTIONS
{
    . = 0;
    _text = .;

    startup : ALIGN(16) SUBALIGN(16)
    {
        KEEP(*(vectors))
    } > flash

    constructors : ALIGN(4) SUBALIGN(4)
    {
        PROVIDE(__init_array_start = .);
        KEEP(*(SORT(.init_array.*)))
        KEEP(*(.init_array))
        PROVIDE(__init_array_end = .);
    } > flash

    destructors : ALIGN(4) SUBALIGN(4)
    {
        PROVIDE(__fini_array_start = .);
        KEEP(*(.fini_array))
        KEEP(*(SORT(.fini_array.*)))
        PROVIDE(__fini_array_end = .);
    } > flash

    .text : ALIGN(16) SUBALIGN(16)
    {
        *(.text.startup.*)
        *(.text)
        *(.text.*)
        *(.rodata)
        *(.rodata.*)
        *(.glue_7t)
        *(.glue_7)
        *(.gcc*)
    } > flash

    .ARM.extab :
    {
        *(.ARM.extab* .gnu.linkonce.armextab.*)
    } > flash

    .ARM.exidx : {
        PROVIDE(__exidx_start = .);
        *(.ARM.exidx* .gnu.linkonce.armexidx.*)
        PROVIDE(__exidx_end = .);
     } > flash

    .eh_frame_hdr :
    {
        *(.eh_frame_hdr)
    } > flash

    .eh_frame : ONLY_IF_RO
    {
        *(.eh_frame)
    } > flash
    
    .textalign : ONLY_IF_RO
    {
        . = ALIGN(8);
    } > flash

    _etext = .;
    _textdata = _etext;

    .stacks :
    {
        . = ALIGN(8);
        __main_stack_base__ = .;
        . += __main_stack_size__;
        . = ALIGN(8);
        __main_stack_end__ = .;
        __process_stack_base__ = .;
        __main_thread_stack_base__ = .;
        . += __process_stack_size__;
        . = ALIGN(8);
        __process_stack_end__ = .;
        __main_thread_stack_end__ = .;
[#if conf.groups.memory_layout_settings.ram_areas_organization[0].@index[0]?string == "0"]
    } > ram
[#else]
    } > ccmram
[/#if]

    .ccm :
    {
        PROVIDE(_cmm_start = .);
        PROVIDE(_cmm_end = .);
    } > ccmram

    .data :
    {
        . = ALIGN(4);
        PROVIDE(_data = .);
        *(.data)
        . = ALIGN(4);
        *(.data.*)
        . = ALIGN(4);
        *(.ramtext)
        . = ALIGN(4);
        PROVIDE(_edata = .);
    } > ram AT > flash

    .bss :
    {
        . = ALIGN(4);
        PROVIDE(_bss_start = .);
        *(.bss)
        . = ALIGN(4);
        *(.bss.*)
        . = ALIGN(4);
        *(COMMON)
        . = ALIGN(4);
        PROVIDE(_bss_end = .);
    } > ram    
}

PROVIDE(end = .);
_end            = .;

__heap_base__   = _end;
__heap_end__    = __ram_end__;
