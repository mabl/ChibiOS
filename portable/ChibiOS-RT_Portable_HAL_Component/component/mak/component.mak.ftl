[#ftl]
[@pp.dropOutputFile /]
[@pp.changeOutputFile name="component.mak" /]
# List of library files exported by the component.
LIB_C_SRC       += ${global.component_path}/lib/src/hal.c \
[#if conf.instance.enabled_drivers.adc[0]?lower_case == "true"]
                   ${global.component_path}/lib/src/adc.c \
[/#if]
[#if conf.instance.enabled_drivers.can[0]?lower_case == "true"]
                   ${global.component_path}/lib/src/can.c \
[/#if]
[#if conf.instance.enabled_drivers.ext[0]?lower_case == "true"]
                   ${global.component_path}/lib/src/ext.c \
[/#if]
[#if conf.instance.enabled_drivers.gpt[0]?lower_case == "true"]
                   ${global.component_path}/lib/src/gpt.c \
[/#if]
[#if conf.instance.enabled_drivers.i2c[0]?lower_case == "true"]
                   ${global.component_path}/lib/src/i2c.c \
[/#if]
[#if conf.instance.enabled_drivers.icu[0]?lower_case == "true"]
                   ${global.component_path}/lib/src/icu.c \
[/#if]
[#if conf.instance.enabled_drivers.mac[0]?lower_case == "true"]
                   ${global.component_path}/lib/src/mac.c \
[/#if]
[#if conf.instance.enabled_drivers.mmc_spi[0]?lower_case == "true"]
                   ${global.component_path}/lib/src/mmc_spi.c \
[/#if]
[#if (conf.instance.enabled_drivers.mmc_spi[0]?lower_case == "true") ||
     (conf.instance.enabled_drivers.sdc[0]?lower_case == "true")]
                   ${global.component_path}/lib/src/mmcsd.c \
[/#if]
[#if conf.instance.enabled_drivers.pal[0]?lower_case == "true"]
                   ${global.component_path}/lib/src/pal.c \
[/#if]
[#if conf.instance.enabled_drivers.pwm[0]?lower_case == "true"]
                   ${global.component_path}/lib/src/pwm.c \
[/#if]
[#if conf.instance.enabled_drivers.rtc[0]?lower_case == "true"]
                   ${global.component_path}/lib/src/rtc.c \
[/#if]
[#if conf.instance.enabled_drivers.sdc[0]?lower_case == "true"]
                   ${global.component_path}/lib/src/sdc.c \
[/#if]
[#if conf.instance.enabled_drivers.serial[0]?lower_case == "true"]
                   ${global.component_path}/lib/src/serial.c \
[/#if]
[#if conf.instance.enabled_drivers.serial_usb[0]?lower_case == "true"]
                   ${global.component_path}/lib/src/serial_usb.c \
[/#if]
[#if conf.instance.enabled_drivers.spi[0]?lower_case == "true"]
                   ${global.component_path}/lib/src/spi.c \
[/#if]
[#if conf.instance.enabled_drivers.tm[0]?lower_case == "true"]
                   ${global.component_path}/lib/src/tm.c \
[/#if]
[#if conf.instance.enabled_drivers.uart[0]?lower_case == "true"]
                   ${global.component_path}/lib/src/uart.c \
[/#if]
[#if conf.instance.enabled_drivers.usb[0]?lower_case == "true"]
                   ${global.component_path}/lib/src/usb.c \
[/#if]

LIB_CPP_SRC     +=

LIB_ASM_SRC     +=

LIB_INCLUDES    += ${global.component_path}/lib/include

APP_C_SRC       +=

APP_CPP_SRC     +=

APP_ASM_SRC     +=

APP_INCLUDES    += ${global.local_component_path}/cfg
