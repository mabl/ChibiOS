[#ftl]
[@pp.dropOutputFile /]
[@pp.changeOutputFile name="component.mak" /]
# List of library files exported by the component.
LIB_C_SRC       += ${global.component_path}/lib/src/test.c \
                   ${global.component_path}/lib/src/testthd.c \
                   ${global.component_path}/lib/src/testsem.c \
                   ${global.component_path}/lib/src/testmtx.c \
                   ${global.component_path}/lib/src/testmsg.c \
                   ${global.component_path}/lib/src/testmbox.c \
                   ${global.component_path}/lib/src/testevt.c \
                   ${global.component_path}/lib/src/testheap.c \
                   ${global.component_path}/lib/src/testpools.c \
                   ${global.component_path}/lib/src/testdyn.c \
                   ${global.component_path}/lib/src/testqueues.c \
                   ${global.component_path}/lib/src/testbmk.c

LIB_CPP_SRC     +=

LIB_ASM_SRC     +=

LIB_INCLUDES    += ${global.component_path}/lib/include

APP_C_SRC       +=

APP_CPP_SRC     +=

APP_ASM_SRC     +=

APP_INCLUDES    +=
