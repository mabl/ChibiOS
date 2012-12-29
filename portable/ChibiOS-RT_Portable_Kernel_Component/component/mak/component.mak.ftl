[#ftl]
[@pp.dropOutputFile /]
[@pp.changeOutputFile name="component.mak" /]
# List of library files exported by the component.
LIB_C_SRC       += ${global.component_path}/lib/src/chsys.c \
                   ${global.component_path}/lib/src/chdebug.c \
                   ${global.component_path}/lib/src/chlists.c \
                   ${global.component_path}/lib/src/chvt.c \
                   ${global.component_path}/lib/src/chschd.c \
                   ${global.component_path}/lib/src/chthreads.c \
                   ${global.component_path}/lib/src/chdynamic.c \
                   ${global.component_path}/lib/src/chregistry.c \
                   ${global.component_path}/lib/src/chsem.c \
                   ${global.component_path}/lib/src/chmtx.c \
                   ${global.component_path}/lib/src/chcond.c \
                   ${global.component_path}/lib/src/chevents.c \
                   ${global.component_path}/lib/src/chmsg.c \
                   ${global.component_path}/lib/src/chmboxes.c \
                   ${global.component_path}/lib/src/chqueues.c \
                   ${global.component_path}/lib/src/chmemcore.c \
                   ${global.component_path}/lib/src/chheap.c \
                   ${global.component_path}/lib/src/chmempools.c

LIB_CPP_SRC     +=

LIB_ASM_SRC     +=

LIB_INCLUDES    += ${global.component_path}/lib/include

APP_C_SRC       +=

APP_CPP_SRC     +=

APP_ASM_SRC     +=

APP_INCLUDES    += ${global.local_component_path}/cfg
