[#ftl]
[@pp.dropOutputFile /]
[@pp.changeOutputFile name="component.mak" /]
# List of library files exported by the component.
LIB_C_SRC       += ${global.component_path}/lib/src/crt1.c

LIB_CPP_SRC     +=

LIB_ASM_SRC     += ${global.component_path}/lib/src/crt0_v7m.S

LIB_INCLUDES    += ${global.component_path}/lib/include \
                   ${global.component_path}/lib/include\device \
                   ${global.component_path}/lib/include\cmsis

APP_C_SRC       +=

APP_CPP_SRC     +=

APP_ASM_SRC     +=

APP_INCLUDES    +=
