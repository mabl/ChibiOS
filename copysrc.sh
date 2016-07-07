#!/bin/bash
chibios=../chibios-trunk
#do_templates=yes

#============================================================================#
# Portable Components.                                                       #
#============================================================================#

# ChibiOS Product License.
echo "ChibiOS Product License files"
cp -f ${chibios}/os/license/*.h                                         ./portable/chibios_product_license_component/component/lib/include/

#============================================================================#
# RT Kernel Components.                                                      #
#============================================================================#

# ChibiOS/RT Kernel for ARMv7-M.
echo "ChibiOS/RT Kernel for ARMv7-M files"
cp -f ${chibios}/os/rt/src/*.c                                          ./rt/chibios_rt_armv7m_component/component/lib/src/
cp -f ${chibios}/os/rt/include/*.h                                      ./rt/chibios_rt_armv7m_component/component/lib/include/
cp -f ${chibios}/os/common/oslib/src/*.c                                ./rt/chibios_rt_armv7m_component/component/lib/src/
cp -f ${chibios}/os/common/oslib/include/*.h                            ./rt/chibios_rt_armv7m_component/component/lib/include/
cp -f ${chibios}/os/common/ports/ARMCMx/chcore.c                        ./rt/chibios_rt_armv7m_component/component/lib/src/
cp -f ${chibios}/os/common/ports/ARMCMx/chcore_v7m.c                    ./rt/chibios_rt_armv7m_component/component/lib/src/
cp -f ${chibios}/os/common/ports/ARMCMx/chcore.h                        ./rt/chibios_rt_armv7m_component/component/lib/include/
cp -f ${chibios}/os/common/ports/ARMCMx/chcore_v7m.h                    ./rt/chibios_rt_armv7m_component/component/lib/include/
cp -f ${chibios}/os/common/ports/ARMCMx/mpu.h                           ./rt/chibios_rt_armv7m_component/component/lib/include/
cp -f ${chibios}/os/common/ports/ARMCMx/compilers/GCC/chcoreasm_v7m.S   ./rt/chibios_rt_armv7m_component/component/lib/src/
cp -f ${chibios}/os/common/ports/ARMCMx/compilers/GCC/*.h               ./rt/chibios_rt_armv7m_component/component/lib/include/

#============================================================================#
# STM32F4xx Components.                                                      #
#============================================================================#

# STM32F4xx Startup.
echo "STM32F4xx Startup files"
cp -f ${chibios}/os/common/startup/ARMCMx/compilers/GCC/rules.mk        ./platforms/STM32F4xx/chibios_stm32f4xx_startup_component/component/lib/rsc/
cp -f ${chibios}/os/common/startup/ARMCMx/compilers/GCC/ld/rules*.ld    ./platforms/STM32F4xx/chibios_stm32f4xx_startup_component/component/lib/rsc/ld
cp -f ${chibios}/os/common/startup/ARMCMx/compilers/GCC/*.c             ./platforms/STM32F4xx/chibios_stm32f4xx_startup_component/component/lib/src/
cp -f ${chibios}/os/common/startup/ARMCMx/compilers/GCC/crt0_v7m.S      ./platforms/STM32F4xx/chibios_stm32f4xx_startup_component/component/lib/src/
cp -f ${chibios}/os/common/startup/ARMCMx/compilers/GCC/*.h             ./platforms/STM32F4xx/chibios_stm32f4xx_startup_component/component/lib/include/
cp -f ${chibios}/os/common/startup/ARMCMx/devices/STM32F4xx/*.h         ./platforms/STM32F4xx/chibios_stm32f4xx_startup_component/component/lib/include/
cp -f ${chibios}/os/common/ext/CMSIS/ST/STM32F4xx/*.h                   ./platforms/STM32F4xx/chibios_stm32f4xx_startup_component/component/lib/include/device
cp -f ${chibios}/os/common/ext/CMSIS/include/*.h                        ./platforms/STM32F4xx/chibios_stm32f4xx_startup_component/component/lib/include/cmsis

# STM32F4xx HAL.
echo "STM32F4xx HAL files"
cp -f ${chibios}/os/hal/src/*.c                                         ./platforms/STM32F4xx/chibios_stm32f4xx_hal_component/component/lib/src/
cp -f ${chibios}/os/hal/include/*.h                                     ./platforms/STM32F4xx/chibios_stm32f4xx_hal_component/component/lib/include/

echo ""
echo "Done!"
