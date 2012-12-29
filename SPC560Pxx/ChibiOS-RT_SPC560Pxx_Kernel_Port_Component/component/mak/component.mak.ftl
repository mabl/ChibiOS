[#ftl]
[@pp.dropOutputFile /]
[@pp.changeOutputFile name="component.mak" /]
# List of library files exported by the component.
LIB_C_SRC       += ${global.component_path}/lib/src/chcore.c

LIB_CPP_SRC     +=

LIB_ASM_SRC     += ${global.component_path}/lib/src/ivor.s \
                   ${global.component_path}/lib/src/vectors.s

LIB_INCLUDES    += ${global.component_path}/lib/include

APP_C_SRC       +=

APP_CPP_SRC     +=

APP_ASM_SRC     +=

APP_INCLUDES    +=
