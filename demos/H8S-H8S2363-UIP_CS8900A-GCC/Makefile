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
# Option not tested on H8S, DO NOT USE.
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
CHIBIOS = ../..
include $(CHIBIOS)/boards/GENERIC_H8S/board.mk
include $(CHIBIOS)/os/hal/platforms/H8S/platform.mk
include $(CHIBIOS)/os/hal/hal.mk
include $(CHIBIOS)/os/ports/GCC/H8S/port.mk
include $(CHIBIOS)/os/kernel/kernel.mk
include $(CHIBIOS)/test/test.mk

# List of the required uIP source files.
USRC = $(CHIBIOS)/ext/uip-1.0/uip/uip_arp.c \
       $(CHIBIOS)/ext/uip-1.0/uip/psock.c \
       $(CHIBIOS)/ext/uip-1.0/uip/uip.c \
       $(CHIBIOS)/ext/uip-1.0/apps/webserver/httpd.c \
       $(CHIBIOS)/ext/uip-1.0/apps/webserver/http-strings.c \
       $(CHIBIOS)/ext/uip-1.0/apps/webserver/httpd-fs.c \
       $(CHIBIOS)/ext/uip-1.0/apps/webserver/httpd-cgi.c

# C sources here.
CSRC = $(PORTSRC) \
       $(KERNSRC) \
       $(TESTSRC) \
       $(HALSRC) \
       $(PLATFORMSRC) \
       $(BOARDSRC) \
       $(USRC) \
       $(CHIBIOS)/os/various/syscalls.c \
       $(CHIBIOS)/os/various/evtimer.c \
       web/webthread.c \
       main.c

# C++ sources here.
CPPSRC =

# List ASM source files here
ASMSRC = $(PORTASM)

INCDIR = $(PORTINC) $(KERNINC) $(TESTINC) \
         $(HALINC) $(PLATFORMINC) $(BOARDINC) \
         $(CHIBIOS)/os/various \
         ./web $(CHIBIOS)/ext/uip-1.0/uip $(CHIBIOS)/ext/uip-1.0/apps/webserver

#
# Project, sources and paths
##############################################################################

##############################################################################
# Compiler settings
#

MCU  = s

TRGT = h8300-elf-
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
HEX  = $(CP) -O srec --srec-forceS3
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
DDEFS = -DF_CPU=32000000
DDEFS += -DROMSTART=1

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

include $(CHIBIOS)/os/ports/GCC/H8S/rules.mk
