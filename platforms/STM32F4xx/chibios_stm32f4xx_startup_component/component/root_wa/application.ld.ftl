[#ftl]
[#import "/@ftllibs/libutils.ftl" as utils /]
[#macro memory_unit s]
  [#if s == "kB"]
k[#rt]
  [#elseif s == "MB"]
M[#rt]
  [#else]
[#rt]
  [/#if]
[/#macro]
[@pp.dropOutputFile /]
[@pp.changeOutputFile name="application.ld" /]
/*
    ChibiOS - Copyright (C) 2006..2016 Giovanni Di Sirio

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
 * ${conf.instance.device_settings.specific_model.value[0]} memory setup.
 */
MEMORY
{
    flash0  : org = ${conf.instance.linker_settings.memory_regions_layout.flash_0_address.value[0]}, len = ${conf.instance.linker_settings.memory_regions_layout.flash_0_size.value[0]}[@memory_unit s=conf.instance.linker_settings.memory_regions_layout.flash_0_size.metadatas.metadata[0].unit.@name[0]?string /]
    flash1  : org = ${conf.instance.linker_settings.memory_regions_layout.flash_1_address.value[0]}, len = ${conf.instance.linker_settings.memory_regions_layout.flash_1_size.value[0]}[@memory_unit s=conf.instance.linker_settings.memory_regions_layout.flash_1_size.metadatas.metadata[0].unit.@name[0]?string /]
    flash2  : org = ${conf.instance.linker_settings.memory_regions_layout.flash_2_address.value[0]}, len = ${conf.instance.linker_settings.memory_regions_layout.flash_2_size.value[0]}[@memory_unit s=conf.instance.linker_settings.memory_regions_layout.flash_2_size.metadatas.metadata[0].unit.@name[0]?string /]
    flash3  : org = ${conf.instance.linker_settings.memory_regions_layout.flash_3_address.value[0]}, len = ${conf.instance.linker_settings.memory_regions_layout.flash_3_size.value[0]}[@memory_unit s=conf.instance.linker_settings.memory_regions_layout.flash_3_size.metadatas.metadata[0].unit.@name[0]?string /]
    flash4  : org = ${conf.instance.linker_settings.memory_regions_layout.flash_4_address.value[0]}, len = ${conf.instance.linker_settings.memory_regions_layout.flash_4_size.value[0]}[@memory_unit s=conf.instance.linker_settings.memory_regions_layout.flash_4_size.metadatas.metadata[0].unit.@name[0]?string /]
    flash5  : org = ${conf.instance.linker_settings.memory_regions_layout.flash_5_address.value[0]}, len = ${conf.instance.linker_settings.memory_regions_layout.flash_5_size.value[0]}[@memory_unit s=conf.instance.linker_settings.memory_regions_layout.flash_5_size.metadatas.metadata[0].unit.@name[0]?string /]
    flash6  : org = ${conf.instance.linker_settings.memory_regions_layout.flash_6_address.value[0]}, len = ${conf.instance.linker_settings.memory_regions_layout.flash_6_size.value[0]}[@memory_unit s=conf.instance.linker_settings.memory_regions_layout.flash_6_size.metadatas.metadata[0].unit.@name[0]?string /]
    flash7  : org = ${conf.instance.linker_settings.memory_regions_layout.flash_7_address.value[0]}, len = ${conf.instance.linker_settings.memory_regions_layout.flash_7_size.value[0]}[@memory_unit s=conf.instance.linker_settings.memory_regions_layout.flash_7_size.metadatas.metadata[0].unit.@name[0]?string /]
    ram0    : org = ${conf.instance.linker_settings.memory_regions_layout.ram_0_address.value[0]}, len = ${conf.instance.linker_settings.memory_regions_layout.ram_0_size.value[0]}[@memory_unit s=conf.instance.linker_settings.memory_regions_layout.ram_0_size.metadatas.metadata[0].unit.@name[0]?string /]
    ram1    : org = ${conf.instance.linker_settings.memory_regions_layout.ram_1_address.value[0]}, len = ${conf.instance.linker_settings.memory_regions_layout.ram_1_size.value[0]}[@memory_unit s=conf.instance.linker_settings.memory_regions_layout.ram_1_size.metadatas.metadata[0].unit.@name[0]?string /]
    ram2    : org = ${conf.instance.linker_settings.memory_regions_layout.ram_2_address.value[0]}, len = ${conf.instance.linker_settings.memory_regions_layout.ram_2_size.value[0]}[@memory_unit s=conf.instance.linker_settings.memory_regions_layout.ram_2_size.metadatas.metadata[0].unit.@name[0]?string /]
    ram3    : org = ${conf.instance.linker_settings.memory_regions_layout.ram_3_address.value[0]}, len = ${conf.instance.linker_settings.memory_regions_layout.ram_3_size.value[0]}[@memory_unit s=conf.instance.linker_settings.memory_regions_layout.ram_3_size.metadatas.metadata[0].unit.@name[0]?string /]
    ram4    : org = ${conf.instance.linker_settings.memory_regions_layout.ram_4_address.value[0]}, len = ${conf.instance.linker_settings.memory_regions_layout.ram_4_size.value[0]}[@memory_unit s=conf.instance.linker_settings.memory_regions_layout.ram_4_size.metadatas.metadata[0].unit.@name[0]?string /]
    ram5    : org = ${conf.instance.linker_settings.memory_regions_layout.ram_5_address.value[0]}, len = ${conf.instance.linker_settings.memory_regions_layout.ram_5_size.value[0]}[@memory_unit s=conf.instance.linker_settings.memory_regions_layout.ram_5_size.metadatas.metadata[0].unit.@name[0]?string /]
    ram6    : org = ${conf.instance.linker_settings.memory_regions_layout.ram_6_address.value[0]}, len = ${conf.instance.linker_settings.memory_regions_layout.ram_6_size.value[0]}[@memory_unit s=conf.instance.linker_settings.memory_regions_layout.ram_6_size.metadatas.metadata[0].unit.@name[0]?string /]
    ram7    : org = ${conf.instance.linker_settings.memory_regions_layout.ram_7_address.value[0]}, len = ${conf.instance.linker_settings.memory_regions_layout.ram_7_size.value[0]}[@memory_unit s=conf.instance.linker_settings.memory_regions_layout.ram_7_size.metadatas.metadata[0].unit.@name[0]?string /]
}

/* For each data/text section two region are defined, a virtual region
   and a load region (_LMA suffix).*/

/* Flash region to be used for exception vectors.*/
REGION_ALIAS("VECTORS_FLASH", ${conf.instance.linker_settings.memory_sections_assignment.vectors_segment_vma.value[0]?lower_case?replace(" ", "")});
REGION_ALIAS("VECTORS_FLASH_LMA", ${conf.instance.linker_settings.memory_sections_assignment.vectors_segment_lma.value[0]?lower_case?replace(" ", "")});

/* Flash region to be used for constructors and destructors.*/
REGION_ALIAS("XTORS_FLASH", ${conf.instance.linker_settings.memory_sections_assignment.xtors_segments_lma.value[0]?lower_case?replace(" ", "")});
REGION_ALIAS("XTORS_FLASH_LMA", ${conf.instance.linker_settings.memory_sections_assignment.xtors_segments_lma.value[0]?lower_case?replace(" ", "")});

/* Flash region to be used for code text.*/
REGION_ALIAS("TEXT_FLASH", ${conf.instance.linker_settings.memory_sections_assignment.text_segment_lma.value[0]?lower_case?replace(" ", "")});
REGION_ALIAS("TEXT_FLASH_LMA", ${conf.instance.linker_settings.memory_sections_assignment.text_segment_vma.value[0]?lower_case?replace(" ", "")});

/* Flash region to be used for read only data.*/
REGION_ALIAS("RODATA_FLASH", ${conf.instance.linker_settings.memory_sections_assignment.rodata_segment_lma.value[0]?lower_case?replace(" ", "")});
REGION_ALIAS("RODATA_FLASH_LMA", ${conf.instance.linker_settings.memory_sections_assignment.rodata_segment_vma.value[0]?lower_case?replace(" ", "")});

/* Flash region to be used for various.*/
REGION_ALIAS("VARIOUS_FLASH", ${conf.instance.linker_settings.memory_sections_assignment.various_segments_lma.value[0]?lower_case?replace(" ", "")});
REGION_ALIAS("VARIOUS_FLASH_LMA", ${conf.instance.linker_settings.memory_sections_assignment.various_segments_vma.value[0]?lower_case?replace(" ", "")});

/* Flash region to be used for RAM(n) initialization data.*/
REGION_ALIAS("RAM_INIT_FLASH_LMA", ${conf.instance.linker_settings.memory_sections_assignment.ram_init_data_segments_lma.value[0]?lower_case?replace(" ", "")});

/* RAM region to be used for Main stack. This stack accommodates the processing
   of all exceptions and interrupts.*/
REGION_ALIAS("MAIN_STACK_RAM", ${conf.instance.linker_settings.memory_sections_assignment.main_stack_segment_ram.value[0]?lower_case?replace(" ", "")});

/* RAM region to be used for the process stack. This is the stack used by
   the main() function.*/
REGION_ALIAS("PROCESS_STACK_RAM", ${conf.instance.linker_settings.memory_sections_assignment.process_stack_segment_ram.value[0]?lower_case?replace(" ", "")});

/* RAM region to be used for data segment.*/
REGION_ALIAS("DATA_RAM", ${conf.instance.linker_settings.memory_sections_assignment.data_segment_ram.value[0]?lower_case?replace(" ", "")});
REGION_ALIAS("DATA_RAM_LMA", ${conf.instance.linker_settings.memory_sections_assignment.data_segment_init_data_lma.value[0]?lower_case?replace(" ", "")});

/* RAM region to be used for BSS segment.*/
REGION_ALIAS("BSS_RAM", ${conf.instance.linker_settings.memory_sections_assignment.bss_segment_ram.value[0]?lower_case?replace(" ", "")});

/* RAM region to be used for the default heap.*/
REGION_ALIAS("HEAP_RAM", ${conf.instance.linker_settings.memory_sections_assignment.heap_ram.value[0]?lower_case?replace(" ", "")});

[@utils.EmitIndentedCCode "" 2 conf.instance.linker_settings.load_rules.sections.value[0] /]
