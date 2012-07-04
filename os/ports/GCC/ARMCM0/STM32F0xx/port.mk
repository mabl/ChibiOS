# List of the Nil RTOS Cortex-M0 STM32 port files.
PORTSRC = $(NILRTOS)/os/ports/GCC/ARMCM0/crt0.c \
          $(NILRTOS)/os/ports/GCC/ARMCM0/STM32F0xx/vectors.c

PORTASM =

PORTINC = ${NILRTOS}/os/ports/GCC/ARMCM0 \
          ${NILRTOS}/os/ports/GCC/ARMCM0/STM32F0xx

PORTLD  = ${NILRTOS}/os/ports/GCC/ARMCM0/STM32F0xx/ld
