# List of the ChibiOS/RT ARM7 BCM2835 port files.
PORTSRC = ${CHIBIOS}/os/ports/GCC/ARM/chcore.c

PORTASM = ${CHIBIOS}/os/ports/GCC/ARM/crt0.s \
          ${CHIBIOS}/os/ports/GCC/ARM/chcoreasm.s \
          ${CHIBIOS}/os/ports/GCC/ARM/BCM2835/vectors.s

PORTINC = ${CHIBIOS}/os/ports/GCC/ARM \
          ${CHIBIOS}/os/ports/GCC/ARM/BCM2835

PORTLD  = ${CHIBIOS}/os/ports/GCC/ARM/BCM2835/ld
