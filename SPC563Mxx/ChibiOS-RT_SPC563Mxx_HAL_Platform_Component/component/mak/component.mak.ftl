[#ftl]
[@pp.dropOutputFile /]
[@pp.changeOutputFile name="component.mak" /]
LIB_C_SRC       += ${global.component_path}/lib/src/spc5_edma.c.c \
                   ${global.component_path}/lib/src/hal_lld.c \
[#if global.instances["org.chibios.spc5.components.hal"].enabled_drivers.adc[0]?lower_case == "true"]
                   ${global.component_path}/lib/src/adc_lld.c \
[/#if]
[#if global.instances["org.chibios.spc5.components.hal"].enabled_drivers.pal[0]?lower_case == "true"]
                   ${global.component_path}/lib/src/pal_lld.c \
[/#if]
[#if global.instances["org.chibios.spc5.components.hal"].enabled_drivers.serial[0]?lower_case == "true"]
                   ${global.component_path}/lib/src/serial_lld.c \
[/#if]

LIB_CPP_SRC     +=

LIB_ASM_SRC     +=

LIB_INCLUDES    += ${global.component_path}/lib/include

APP_C_SRC       +=

APP_CPP_SRC     +=

APP_ASM_SRC     +=

APP_INCLUDES    += ${global.local_component_path}/cfg
