[#ftl]
[@pp.dropOutputFile /]
[@pp.changeOutputFile name="component.mak" /]
# List of library files exported by the component.
LIB_C_SRC       += ${global.component_path}/lib/src/nvic.c \
                   ${global.component_path}/lib/src/chcore.c \
                   ${global.component_path}/lib/src/chcore_v7m.c \
                   ${global.component_path}/lib/src/vectors.c

LIB_CPP_SRC     +=

LIB_ASM_SRC     +=

LIB_INCLUDES    += ${global.component_path}/lib/include

APP_C_SRC       +=

APP_CPP_SRC     +=

APP_ASM_SRC     +=

APP_INCLUDES    +=
