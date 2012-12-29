[#ftl]
[@pp.dropOutputFile /]
[@pp.changeOutputFile name="component.mak" /]
LIB_C_SRC       += ${global.component_path}/lib/src/hal_lld.c \
                   ${global.component_path}/lib/src/pal_lld.c \
                   ${global.component_path}/lib/src/serial_lld.c

LIB_CPP_SRC     +=

LIB_ASM_SRC     +=

LIB_INCLUDES    += ${global.component_path}/lib/include

APP_C_SRC       +=

APP_CPP_SRC     +=

APP_ASM_SRC     +=

APP_INCLUDES    += ${global.local_component_path}/cfg
