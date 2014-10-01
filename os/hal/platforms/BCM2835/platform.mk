# List of all the BCM2835 platform files.
PLATFORMSRC = ${CHIBIOS}/os/hal/platforms/BCM2835/hal_lld.c \
              ${CHIBIOS}/os/hal/platforms/BCM2835/pal_lld.c \
              ${CHIBIOS}/os/hal/platforms/BCM2835/serial_lld.c \
              ${CHIBIOS}/os/hal/platforms/BCM2835/i2c_lld.c \
              ${CHIBIOS}/os/hal/platforms/BCM2835/spi_lld.c \
              ${CHIBIOS}/os/hal/platforms/BCM2835/gpt_lld.c \
              ${CHIBIOS}/os/hal/platforms/BCM2835/pwm_lld.c \
              ${CHIBIOS}/os/hal/platforms/BCM2835/bcm2835.c

# Required include directories
PLATFORMINC = ${CHIBIOS}/os/hal/platforms/BCM2835
