[#ftl]
[@pp.dropOutputFile /]
[@pp.changeOutputFile name="component.mak" /]
LIB_C_SRC       += ${global.component_path}/lib/src/stm32_dma.c \
                   ${global.component_path}/lib/src/hal_lld.c \
[#if global.instances["org.chibios.spc5.components.hal"].enabled_drivers.adc[0]?lower_case == "true"]
                   ${global.component_path}/lib/src/adc_lld.c \
[/#if]
[#if global.instances["org.chibios.spc5.components.hal"].enabled_drivers.can[0]?lower_case == "true"]
                   ${global.component_path}/lib/src/can_lld.c \
[/#if]
[#if global.instances["org.chibios.spc5.components.hal"].enabled_drivers.ext[0]?lower_case == "true"]
                   ${global.component_path}/lib/src/ext_lld.c \
                   ${global.component_path}/lib/src/ext_lld_isr.c \
[/#if]
[#if global.instances["org.chibios.spc5.components.hal"].enabled_drivers.gpt[0]?lower_case == "true"]
                   ${global.component_path}/lib/src/gpt_lld.c \
[/#if]
[#if global.instances["org.chibios.spc5.components.hal"].enabled_drivers.i2c[0]?lower_case == "true"]
                   ${global.component_path}/lib/src/i2c_lld.c \
[/#if]
[#if global.instances["org.chibios.spc5.components.hal"].enabled_drivers.icu[0]?lower_case == "true"]
                   ${global.component_path}/lib/src/icu_lld.c \
[/#if]
[#if global.instances["org.chibios.spc5.components.hal"].enabled_drivers.mac[0]?lower_case == "true"]
                   ${global.component_path}/lib/src/mac_lld.c \
[/#if]
[#if global.instances["org.chibios.spc5.components.hal"].enabled_drivers.pal[0]?lower_case == "true"]
                   ${global.component_path}/lib/src/pal_lld.c \
[/#if]
[#if global.instances["org.chibios.spc5.components.hal"].enabled_drivers.pwm[0]?lower_case == "true"]
                   ${global.component_path}/lib/src/pwm_lld.c \
[/#if]
[#if global.instances["org.chibios.spc5.components.hal"].enabled_drivers.serial[0]?lower_case == "true"]
                   ${global.component_path}/lib/src/serial_lld.c \
[/#if]
[#if global.instances["org.chibios.spc5.components.hal"].enabled_drivers.sdc[0]?lower_case == "true"]
                   ${global.component_path}/lib/src/sdc_lld.c \
[/#if]
[#if global.instances["org.chibios.spc5.components.hal"].enabled_drivers.rtc[0]?lower_case == "true"]
                   ${global.component_path}/lib/src/rtc_lld.c \
[/#if]
[#if global.instances["org.chibios.spc5.components.hal"].enabled_drivers.spi[0]?lower_case == "true"]
                   ${global.component_path}/lib/src/spi_lld.c \
[/#if]
[#if global.instances["org.chibios.spc5.components.hal"].enabled_drivers.uart[0]?lower_case == "true"]
                   ${global.component_path}/lib/src/uart_lld.c \
[/#if]
[#if global.instances["org.chibios.spc5.components.hal"].enabled_drivers.usb[0]?lower_case == "true"]
                   ${global.component_path}/lib/src/usb_lld.c \
[/#if]

LIB_CPP_SRC     +=

LIB_ASM_SRC     +=

LIB_INCLUDES    += ${global.component_path}/lib/include

APP_C_SRC       +=

APP_CPP_SRC     +=

APP_ASM_SRC     +=

APP_INCLUDES    += ${global.local_component_path}/cfg
