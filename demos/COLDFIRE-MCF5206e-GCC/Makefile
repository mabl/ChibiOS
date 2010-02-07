##############################################################################
# Build global options
# NOTE: Can be overridden externally.
#

# Compiler options here.
ifeq ($(USE_OPT),)
  USE_OPT = -Os -ggdb -fomit-frame-pointer
endif

# C++ specific options here (added to USE_OPT).
ifeq ($(USE_CPPOPT),)
  USE_CPPOPT = -fno-rtti
endif

# Enable this if you want the linker to remove unused code and data
ifeq ($(USE_LINK_GC),)
  USE_LINK_GC = yes
endif

# Enable register caching optimization (read documentation).
# Option not tested on ColdFire, DO NOT USE.
ifeq ($(USE_CURRP_CACHING),)
  USE_CURRP_CACHING = no
endif

#
# Build global options
##############################################################################

##############################################################################
# Project, sources and paths
#

# Define project name here
PROJECT = ch

# Define linker script file here
LDSCRIPT= ch.ld

# Imported source files
include ../../src/kernel.mk
include ../../test/test.mk

# C sources here.
CSRC = ../../ports/ColdFire/chcore.c \
       ../../ports/ColdFire/coldfire_serial.c \
       ${KERNSRC} \
       ${TESTSRC} \
       board.c main.c

# C++ sources here.
CPPSRC =

# List ASM source files here
ASMSRC = ../../ports/ColdFire/crt0.s

INCDIR = $(KERNINC) $(TESTINC) \
         ../../src/lib \
         ../../ports/ColdFire

#
# Project, sources and paths
##############################################################################

##############################################################################
# Compiler settings
#

MCU  = 5200

TRGT = m68k-elf-
CC   = $(TRGT)gcc
CPPC = $(TRGT)g++
# Enable loading with g++ only if you need C++ runtime support.
# NOTE: You can use C++ even without C++ support if you are careful. C++
#       runtime support makes code size explode.
LD   = $(TRGT)gcc
#LD   = $(TRGT)g++
CP   = $(TRGT)objcopy
AS   = $(TRGT)gcc -x assembler-with-cpp
OD   = $(TRGT)objdump
HEX  = $(CP) -O srec
BIN  = $(CP) -O binary

# Define C warning options here
CWARN = -Wall -Wstrict-prototypes

# Define C++ warning options here
CPPWARN = -Wall

#
# Compiler settings
##############################################################################

##############################################################################
# Start of default section
#

# List all default C defines here, like -D_DEBUG=1
DDEFS = -DF_CPU=50000000

# List all default ASM defines here, like -D_DEBUG=1
DADEFS =

# List all default directories to look for include files here
DINCDIR =

# List the default directory to look for the libraries here
DLIBDIR =

# List all default libraries here
DLIBS =

#
# End of default section
##############################################################################

##############################################################################
# Start of user section
#

# List all user C define here, like -D_DEBUG=1
UDEFS =

# Define ASM defines here
UADEFS =

# List all user directories here
UINCDIR =

# List the user directory to look for the libraries here
ULIBDIR =

# List all user libraries here
ULIBS =

#
# End of user defines
##############################################################################

include ../../ports/ColdFire/rules.mk
