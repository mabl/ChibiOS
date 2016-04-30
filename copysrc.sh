#!/bin/bash
chibios=../chibios_trunk
#chibios=../../ChibiStudio/chibios_trunk
#chibios=../../ChibiStudio/chibios
#do_templates=yes

#============================================================================#
# Portable Components.                                                       #
#============================================================================#

#============================================================================#
# RT Kernel Components.                                                      #
#============================================================================#

# STM32F4xx RT Kernel for ARMv7-M.
echo "STM32F4xx RT Kernel for ARMv7-M files"
cp -f ${chibios}/os/rt/src/*.c                                              ./rt/chibios_rt_armv7m_component/component/lib/src/
cp -f ${chibios}/os/rt/include/*.h                                          ./rt/chibios_rt_armv7m_component/component/lib/include/
cp -f ${chibios}/os/common/ports/ARMCMx/chcore.c                            ./rt/chibios_rt_armv7m_component/component/lib/src/
cp -f ${chibios}/os/common/ports/ARMCMx/chcore_v7m.c                        ./rt/chibios_rt_armv7m_component/component/lib/src/
cp -f ${chibios}/os/common/ports/ARMCMx/chcore.h                            ./rt/chibios_rt_armv7m_component/component/lib/include/
cp -f ${chibios}/os/common/ports/ARMCMx/chcore_v7m.h                        ./rt/chibios_rt_armv7m_component/component/lib/include/
cp -f ${chibios}/os/common/ports/ARMCMx/compilers/GCC/chcoreasm_v7m.S       ./rt/chibios_rt_armv7m_component/component/lib/src/
cp -f ${chibios}/os/common/ports/ARMCMx/compilers/GCC/*.h                   ./rt/chibios_rt_armv7m_component/component/lib/include/

#============================================================================#
# STM32F4xx Components.                                                      #
#============================================================================#

# STM32F4xx Startup.
echo "STM32F4xx Startup files"
cp -f ${chibios}/os/common/startup/ARMCMx/compilers/GCC/rules.mk            ./platforms/STM32F4xx/chibios_stm32f4xx_startup_component/component/lib/rsc/
cp -f ${chibios}/os/common/startup/ARMCMx/compilers/GCC/ld/rules*.ld        ./platforms/STM32F4xx/chibios_stm32f4xx_startup_component/component/lib/rsc/ld
cp -f ${chibios}/os/common/startup/ARMCMx/compilers/GCC/*.c                 ./platforms/STM32F4xx/chibios_stm32f4xx_startup_component/component/lib/src/
cp -f ${chibios}/os/common/startup/ARMCMx/compilers/GCC/crt0_v7m.S          ./platforms/STM32F4xx/chibios_stm32f4xx_startup_component/component/lib/src/
cp -f ${chibios}/os/common/startup/ARMCMx/compilers/GCC/*.h                 ./platforms/STM32F4xx/chibios_stm32f4xx_startup_component/component/lib/include/
cp -f ${chibios}/os/common/startup/ARMCMx/devices/STM32F4xx/*.h             ./platforms/STM32F4xx/chibios_stm32f4xx_startup_component/component/lib/include/
cp -f ${chibios}/os/common/ext/CMSIS/ST/STM32F4xx/*.h                       ./platforms/STM32F4xx/chibios_stm32f4xx_startup_component/component/lib/include/device
cp -f ${chibios}/os/common/ext/CMSIS/include/*.h                            ./platforms/STM32F4xx/chibios_stm32f4xx_startup_component/component/lib/include/cmsis


echo ""
echo "Done!"
