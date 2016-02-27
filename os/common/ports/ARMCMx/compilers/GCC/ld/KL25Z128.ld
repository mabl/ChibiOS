/*
    ChibiOS - Copyright (C) 2006..2015 Giovanni Di Sirio

    Licensed under the Apache License, Version 2.0 (the "License");
    you may not use this file except in compliance with the License.
    You may obtain a copy of the License at

        http://www.apache.org/licenses/LICENSE-2.0

    Unless required by applicable law or agreed to in writing, software
    distributed under the License is distributed on an "AS IS" BASIS,
    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
    See the License for the specific language governing permissions and
    limitations under the License.
*/

/*
 * KL25Z128 memory setup.
 */

ENTRY(Reset_Handler)

MEMORY
{
  flash0   :  org = 0x00000000,  len = 0xc0
  flashcfg :  org = 0x00000400,  len = 0x10
  flash    :  org = 0x00000410,  len = 128k - 0x410
  ram      :  org = 0x1FFFF000,  len = 16k
}

__ram_start__           = ORIGIN(ram);
__ram_size__            = LENGTH(ram);
__ram_end__             = __ram_start__ + __ram_size__;

SECTIONS
{
  . = 0;

  .isr : ALIGN(4) SUBALIGN(4)
  {
    KEEP(*(.vectors))
  } > flash0

  .cfmprotect : ALIGN(4) SUBALIGN(4)
  {
    KEEP(*(.cfmconfig))
  } > flashcfg

  _text = .;

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

  .text : ALIGN(4) SUBALIGN(4)
  {
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
  } > ram

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
