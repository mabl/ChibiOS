[#ftl]
[@pp.dropOutputFile /]
[@pp.changeOutputFile name="component.mak" /]
# List of library files exported by the component.
LIB_C_SRC       += ${global.component_path}/lib/src/hal.c \
                   ${global.component_path}/lib/src/adc.c \
                   ${global.component_path}/lib/src/can.c \
                   ${global.component_path}/lib/src/ext.c \
                   ${global.component_path}/lib/src/gpt.c \
                   ${global.component_path}/lib/src/i2c.c \
                   ${global.component_path}/lib/src/icu.c \
                   ${global.component_path}/lib/src/mac.c \
                   ${global.component_path}/lib/src/mmc_spi.c \
                   ${global.component_path}/lib/src/mmcsd.c \
                   ${global.component_path}/lib/src/pal.c \
                   ${global.component_path}/lib/src/pwm.c \
                   ${global.component_path}/lib/src/rtc.c \
                   ${global.component_path}/lib/src/sdc.c \
                   ${global.component_path}/lib/src/serial.c \
                   ${global.component_path}/lib/src/serial_usb.c \
                   ${global.component_path}/lib/src/spi.c \
                   ${global.component_path}/lib/src/tm.c \
                   ${global.component_path}/lib/src/uart.c \
                   ${global.component_path}/lib/src/usb.c

LIB_CPP_SRC     +=

LIB_ASM_SRC     +=

LIB_INCLUDES    += ${global.component_path}/lib/include

APP_C_SRC       +=

APP_CPP_SRC     +=

APP_ASM_SRC     +=

APP_INCLUDES    += ${global.local_component_path}/cfg
