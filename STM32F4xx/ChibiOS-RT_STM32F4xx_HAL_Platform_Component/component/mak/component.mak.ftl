[#ftl]
[@pp.dropOutputFile /]
[@pp.changeOutputFile name="component.mak" /]
LIB_C_SRC       += ${global.component_path}/lib/src/stm32_dma.c \
                   ${global.component_path}/lib/src/hal_lld.c \
                   ${global.component_path}/lib/src/adc_lld.c \
                   ${global.component_path}/lib/src/ext_lld_isr.c \
                   ${global.component_path}/lib/src/can_lld.c \
                   ${global.component_path}/lib/src/ext_lld.c \
                   ${global.component_path}/lib/src/gpt_lld.c \
                   ${global.component_path}/lib/src/icu_lld.c \
                   ${global.component_path}/lib/src/mac_lld.c \
                   ${global.component_path}/lib/src/pwm_lld.c \
                   ${global.component_path}/lib/src/sdc_lld.c \
                   ${global.component_path}/lib/src/GPIOv2/pal_lld.c \
                   ${global.component_path}/lib/src/I2Cv1/i2c_lld.c \
                   ${global.component_path}/lib/src/OTGv1/usb_lld.c \
                   ${global.component_path}/lib/src/RTCv2/rtc_lld.c \
                   ${global.component_path}/lib/src/SPIv1/spi_lld.c \
                   ${global.component_path}/lib/src/USARTv1/serial_lld.c \
                   ${global.component_path}/lib/src/USARTv1/uart_lld.c

LIB_CPP_SRC     +=

LIB_ASM_SRC     +=

LIB_INCLUDES    += ${global.component_path}/lib/include

APP_C_SRC       +=

APP_CPP_SRC     +=

APP_ASM_SRC     +=

APP_INCLUDES    += ${global.local_component_path}/cfg
