#!/bin/bash
#chibios=../chibios_trunk
#chibios=../../ChibiStudio/chibios_trunk
chibios=../../ChibiStudio/chibios
#do_templates=yes

#============================================================================#
# Portable Components.                                                       #
#============================================================================#

#============================================================================#
# STM32F4xx  Components.                                                     #
#============================================================================#

# STM32F4xx Startup.
echo "STM32F4xx Startup files"
cp -f ${chibios}/os/common/startup/ARMCMx/compilers/GCC/ld/rules_*.ld       ./platforms/STM32F4xx/chibios_stm32f4xx_startup_component/component/lib/rsc/
cp -f ${chibios}/os/common/startup/ARMCMx/compilers/GCC/rules.mk            ./platforms/STM32F4xx/chibios_stm32f4xx_startup_component/component/lib/rsc/
cp -f ${chibios}/os/common/startup/ARMCMx/compilers/GCC/*.c                 ./platforms/STM32F4xx/chibios_stm32f4xx_startup_component/component/lib/src/
cp -f ${chibios}/os/common/startup/ARMCMx/compilers/GCC/crt0_v7m.s          ./platforms/STM32F4xx/chibios_stm32f4xx_startup_component/component/lib/src/
cp -f ${chibios}/os/common/startup/ARMCMx/compilers/GCC/*.h                 ./platforms/STM32F4xx/chibios_stm32f4xx_startup_component/component/lib/include/
cp -f ${chibios}/os/common/startup/ARMCMx/devices/STM32F4xx/*.h             ./platforms/STM32F4xx/chibios_stm32f4xx_startup_component/component/lib/include/
cp -f ${chibios}/os/common/ext/CMSIS/ST/STM32F4xx/*.h                       ./platforms/STM32F4xx/chibios_stm32f4xx_startup_component/component/lib/include/

echo ""
echo "Done!"
