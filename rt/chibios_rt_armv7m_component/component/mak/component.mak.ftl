[#ftl]
[@pp.dropOutputFile /]
[@pp.changeOutputFile name="component.mak" /]
# List of library files exported by the component.
LIB_C_SRC       += ${global.component_path}/lib/src/chsys.c \
                   ${global.component_path}/lib/src/chdebug.c \
                   ${global.component_path}/lib/src/chtrace.c \
                   ${global.component_path}/lib/src/chvt.c \
                   ${global.component_path}/lib/src/chschd.c \
                   ${global.component_path}/lib/src/chthreads.c \
[#if conf.instance.subsystems.use_time_measurement.value[0]?lower_case == "true"]
                   ${global.component_path}/lib/src/chtm.c \
[/#if]
[#if conf.instance.debug_options.enable_statistics.value[0]?lower_case == "true"]
                   ${global.component_path}/lib/src/chstats.c \
[/#if]
[#if conf.instance.subsystems.use_registry.value[0]?lower_case == "true"]
                   ${global.component_path}/lib/src/chregistry.c \
[/#if]
[#if conf.instance.subsystems.use_semaphores.value[0]?lower_case == "true"]
                   ${global.component_path}/lib/src/chsem.c \
[/#if]
[#if conf.instance.subsystems.use_mutexes.value[0]?lower_case == "true"]
                   ${global.component_path}/lib/src/chmtx.c \
[/#if]
[#if conf.instance.subsystems.use_condition_variables.value[0]?lower_case == "true"]
                   ${global.component_path}/lib/src/chcond.c \
[/#if]
[#if conf.instance.subsystems.use_events.value[0]?lower_case == "true"]
                   ${global.component_path}/lib/src/chevents.c \
[/#if]
[#if conf.instance.subsystems.use_messages.value[0]?lower_case == "true"]
                   ${global.component_path}/lib/src/chmsg.c \
[/#if]
[#if conf.instance.subsystems.use_mailboxes.value[0]?lower_case == "true"]
                   ${global.component_path}/lib/src/chmboxes.c \
[/#if]
[#if conf.instance.memory_management.use_memory_core_allocator.value[0]?lower_case == "true"]
                   ${global.component_path}/lib/src/chmemcore.c \
[/#if]
[#if conf.instance.memory_management.use_heap_allocator.value[0]?lower_case == "true"]
                   ${global.component_path}/lib/src/chheap.c \
[/#if]
[#if conf.instance.memory_management.use_memory_pools_allocator.value[0]?lower_case == "true"]
                   ${global.component_path}/lib/src/chmempools.c \
[/#if]
[#if conf.instance.memory_management.use_dynamic_extensions.value[0]?lower_case == "true"]
                   ${global.component_path}/lib/src/chdynamic.c
[/#if]

LIB_CPP_SRC     +=

LIB_ASM_SRC     +=

LIB_INCLUDES    += ${global.component_path}/lib/include

APP_C_SRC       +=

APP_CPP_SRC     +=

APP_ASM_SRC     +=

APP_INCLUDES    += ${global.local_component_path}/cfg
