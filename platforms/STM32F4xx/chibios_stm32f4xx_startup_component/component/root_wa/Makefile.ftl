[#ftl]
[@pp.dropOutputFile /]
[@pp.changeOutputFile name="Makefile" /]
##############################################################################
# Build global options
# NOTE: Can be overridden externally.
#

# Compiler options here.
ifeq ($(USE_OPT),)
  USE_OPT = ${conf.instance.compiler_settings.optimization_level.value[0]} ${conf.instance.compiler_settings.common_options.value[0]}
endif

# C specific options here (added to USE_OPT).
ifeq ($(USE_COPT),)
  USE_COPT = ${conf.instance.compiler_settings.c_options.value[0]}
endif

# C++ specific options here (added to USE_OPT).
ifeq ($(USE_CPPOPT),)
  USE_CPPOPT = ${conf.instance.compiler_settings.cpp_options.value[0]}
endif

# Enable this if you want the linker to remove unused code and data
ifeq ($(USE_LINK_GC),)
[#if conf.instance.linker_settings.use_linker_gc.value[0]?string == "true"]
  USE_LINK_GC = yes
[#else]
  USE_LINK_GC = no
[/#if]
endif

# Linker extra options here.
ifeq ($(USE_LDOPT),)
  USE_LDOPT = ${conf.instance.linker_settings.linker_options.value[0]}
endif

# Enable this if you want link time optimizations (LTO)
ifeq ($(USE_LTO),)
  USE_LTO = yes
endif

# If enabled, this option allows to compile the application in THUMB mode.
ifeq ($(USE_THUMB),)
  USE_THUMB = yes
endif

# Enable this if you want to see the full log while compiling.
ifeq ($(USE_VERBOSE_COMPILE),)
[#if conf.instance.build_settings.verbose_compile.value[0]?string == "true"]
  USE_VERBOSE_COMPILE = yes
[#else]
  USE_VERBOSE_COMPILE = no
[/#if]
endif

# If enabled, this option makes the build process faster by not compiling
# modules not used in the current configuration.
ifeq ($(USE_SMART_BUILD),)
  USE_SMART_BUILD = yes
endif

#
# Build global options
##############################################################################

##############################################################################
# Architecture or project specific options
#

# Stack size to be allocated to the Cortex-M process stack. This stack is
# the stack used by the main() thread.
ifeq ($(USE_PROCESS_STACKSIZE),)
  USE_PROCESS_STACKSIZE = ${conf.instance.runtime_settings.c_stack_size.value[0]}
endif

# Stack size to the allocated to the Cortex-M main/exceptions stack. This
# stack is used for processing interrupts and exceptions.
ifeq ($(USE_EXCEPTIONS_STACKSIZE),)
  USE_EXCEPTIONS_STACKSIZE = ${conf.instance.runtime_settings.irq_stack_size.value[0]}
endif

# Enables the use of FPU (no, softfp, hard).
ifeq ($(USE_FPU),)
  USE_FPU = ${conf.instance.compiler_settings.enable_fpu.value[0]}
endif

#
# Architecture or project specific options
##############################################################################

##############################################################################
# Project, sources and paths
#

# Define project name here
PROJECT = ${conf.instance.build_settings.application_name.value[0]}

# Imported source files
include components/components.mak

# Checks if there is a user mak file in the project directory.
ifneq ($(wildcard user.mak),) 
    include user.mak
endif

# Define linker script file here
LDSCRIPT= application.ld

# C sources here.
CSRC =      ./components/components.c \
            $(LIB_C_SRC) \
            $(APP_C_SRC) \
            $(U_C_SRC)

# C++ sources here.
CPPSRC =    $(LIB_CPP_SRC) \
            $(APP_CPP_SRC) \
            $(U_CPP_SRC)

# List ASM source files here
ASMXSRC =   $(LIB_ASM_SRC) \
            $(APP_ASM_SRC) \
            $(U_ASM_SRC)

INCDIR =    ./components \
            $(LIB_INCLUDES) \
            $(APP_INCLUDES)

#
# Project, sources and paths
##############################################################################

##############################################################################
# Compiler settings
#

MCU  = cortex-m4

TRGT = ${conf.instance.build_settings.compiler_variant.value[0]}
CC   = $(TRGT)gcc
CPPC = $(TRGT)g++
# Enable loading with g++ only if you need C++ runtime support.
# NOTE: You can use C++ even without C++ support if you are careful. C++
#       runtime support makes code size explode.
LD   = $(TRGT)gcc
#LD   = $(TRGT)g++
CP   = $(TRGT)objcopy
AS   = $(TRGT)gcc -x assembler-with-cpp
AR   = $(TRGT)ar
OD   = $(TRGT)objdump
SZ   = $(TRGT)size
HEX  = $(CP) -O ihex
BIN  = $(CP) -O binary

# ARM-specific options here
AOPT =

# THUMB-specific options here
TOPT = -mthumb -DTHUMB

# Define C warning options here
CWARN = -Wall -Wextra -Wundef -Wstrict-prototypes

# Define C++ warning options here
CPPWARN = -Wall -Wextra -Wundef

#
# Compiler settings
##############################################################################

##############################################################################
# Start of default section
#

# List all default C defines here, like -D_DEBUG=1
DDEFS   = -D${conf.instance.device_settings.specific_model.value[0]}

# List all default ASM defines here, like -D_DEBUG=1
DADEFS  =

# List all default directories to look for include files here
DINCDIR =

# List the default directory to look for the libraries here
DLIBDIR =

# List all default libraries here
DLIBS   =

#
# End of default section
##############################################################################

RULESPATH = ./components/chibios_stm32f4xx_startup_component/lib/rsc
include $(RULESPATH)/rules.mk
