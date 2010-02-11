# List of the ChibiOS/RT PPC port files.
PORTSRC = ${CHIBIOS}/os/ports/GCC/PPC/chcore.c

PORTASM = ${CHIBIOS}/os/ports/GCC/PPC/crt0.s \
          ${CHIBIOS}/os/ports/GCC/PPC/vectors.s \
          ${CHIBIOS}/os/ports/GCC/PPC/ivor4.s

PORTINC = ${CHIBIOS}/os/ports/GCC/PPC
