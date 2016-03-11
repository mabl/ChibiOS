#!/bin/bash
#chibios=/cygdrive/d/Progetti/ChibiOS-RT
#chibios=/cygdrive/d/Progetti/ChibiOS-RT-stable-2.6.x
#chibios=../../chibios-stable-2.6.x
chibios=../dev_2.6.x_gcc_4_9_2
#do_templates=yes

#============================================================================#
# Portable Components.                                                       #
#============================================================================#

# Portable Kernel.
echo "Portable Kernel"
if [ ! -d $do_templates ]
then
  cp -f ${chibios}/os/kernel/templates/chconf.h ./portable/ChibiOS-RT_Portable_Kernel_Component/component/cfg/chconf.h.ftl
fi
cp -f -R ${chibios}/os/kernel/include ./portable/ChibiOS-RT_Portable_Kernel_Component/component/lib
cp -f -R ${chibios}/os/kernel/src ./portable/ChibiOS-RT_Portable_Kernel_Component/component/lib

# Portable HAL.
echo "Portable HAL"
if [ ! -d $do_templates ]
then
  cp -f ${chibios}/os/hal/templates/halconf.h ./portable/ChibiOS-RT_Portable_HAL_Component/component/cfg/halconf.h.ftl
fi
cp -f -R ${chibios}/os/hal/include ./portable/ChibiOS-RT_Portable_HAL_Component/component/lib
cp -f -R ${chibios}/os/hal/src ./portable/ChibiOS-RT_Portable_HAL_Component/component/lib

# Portable Kernel Test Suite.
echo "Portable Test Suite"
cp -f ${chibios}/test/*.h ./portable/ChibiOS-RT_Portable_Kernel_Test_Suite_Component/component/lib/include/
cp -f ${chibios}/test/*.c ./portable/ChibiOS-RT_Portable_Kernel_Test_Suite_Component/component/lib/src/

# Portable Output Formatter.
echo "Portable Output Formatter"
cp -f ${chibios}/os/various/chprintf.h ./portable/ChibiOS-RT_Portable_Output_Formatter_Component/component/lib/include/
cp -f ${chibios}/os/various/chprintf.c ./portable/ChibiOS-RT_Portable_Output_Formatter_Component/component/lib/src/
cp -f ${chibios}/os/various/memstreams.h ./portable/ChibiOS-RT_Portable_Output_Formatter_Component/component/lib/include/
cp -f ${chibios}/os/various/memstreams.c ./portable/ChibiOS-RT_Portable_Output_Formatter_Component/component/lib/src/

# Portable Shell.
echo "Portable Shell"
cp -f ${chibios}/os/various/shell.h ./portable/ChibiOS-RT_Portable_Mini_Shell_Component/component/lib/include/
cp -f ${chibios}/os/various/shell.c ./portable/ChibiOS-RT_Portable_Mini_Shell_Component/component/lib/src/

#============================================================================#
# SPC560BCxx Components.                                                     #
#============================================================================#

# SPC560BCxx Kernel Port.
echo "SPC560BCxx Kernel Port"
cp -f ${chibios}/os/ports/GCC/PPC/*.h                      ./SPC560BCxx/ChibiOS-RT_SPC560BCxx_Kernel_Port_Component/component/lib/include/
cp -f ${chibios}/os/ports/GCC/PPC/SPC560BCxx/*.h           ./SPC560BCxx/ChibiOS-RT_SPC560BCxx_Kernel_Port_Component/component/lib/include/
cp -f ${chibios}/os/ports/GCC/PPC/*.c                      ./SPC560BCxx/ChibiOS-RT_SPC560BCxx_Kernel_Port_Component/component/lib/src/
cp -f ${chibios}/os/ports/GCC/PPC/ivor.s                   ./SPC560BCxx/ChibiOS-RT_SPC560BCxx_Kernel_Port_Component/component/lib/src/
cp -f ${chibios}/os/ports/GCC/PPC/SPC560BCxx/vectors.s     ./SPC560BCxx/ChibiOS-RT_SPC560BCxx_Kernel_Port_Component/component/lib/src/

#============================================================================#
# SPC560Bxx Components.                                                      #
#============================================================================#

# SPC560Bxx Kernel Port.
echo "SPC560Bxx Kernel Port"
cp -f ${chibios}/os/ports/GCC/PPC/*.h                      ./SPC560Bxx/ChibiOS-RT_SPC560Bxx_Kernel_Port_Component/component/lib/include/
cp -f ${chibios}/os/ports/GCC/PPC/SPC560Bxx/*.h            ./SPC560Bxx/ChibiOS-RT_SPC560Bxx_Kernel_Port_Component/component/lib/include/
cp -f ${chibios}/os/ports/GCC/PPC/*.c                      ./SPC560Bxx/ChibiOS-RT_SPC560Bxx_Kernel_Port_Component/component/lib/src/
cp -f ${chibios}/os/ports/GCC/PPC/ivor.s                   ./SPC560Bxx/ChibiOS-RT_SPC560Bxx_Kernel_Port_Component/component/lib/src/
cp -f ${chibios}/os/ports/GCC/PPC/SPC560Bxx/vectors.s      ./SPC560Bxx/ChibiOS-RT_SPC560Bxx_Kernel_Port_Component/component/lib/src/

#============================================================================#
# SPC560Dxx Components.                                                      #
#============================================================================#

# SPC560Dxx Kernel Port.
echo "SPC560Dxx Kernel Port"
cp -f ${chibios}/os/ports/GCC/PPC/*.h                      ./SPC560Dxx/ChibiOS-RT_SPC560Dxx_Kernel_Port_Component/component/lib/include/
cp -f ${chibios}/os/ports/GCC/PPC/SPC560Dxx/*.h            ./SPC560Dxx/ChibiOS-RT_SPC560Dxx_Kernel_Port_Component/component/lib/include/
cp -f ${chibios}/os/ports/GCC/PPC/*.c                      ./SPC560Dxx/ChibiOS-RT_SPC560Dxx_Kernel_Port_Component/component/lib/src/
cp -f ${chibios}/os/ports/GCC/PPC/ivor.s                   ./SPC560Dxx/ChibiOS-RT_SPC560Dxx_Kernel_Port_Component/component/lib/src/
cp -f ${chibios}/os/ports/GCC/PPC/SPC560Dxx/vectors.s      ./SPC560Dxx/ChibiOS-RT_SPC560Dxx_Kernel_Port_Component/component/lib/src/

#============================================================================#
# SPC560Pxx Components.                                                      #
#============================================================================#

# SPC560Pxx Kernel Port.
echo "SPC560Pxx Kernel Port"
cp -f ${chibios}/os/ports/GCC/PPC/*.h                      ./SPC560Pxx/ChibiOS-RT_SPC560Pxx_Kernel_Port_Component/component/lib/include/
cp -f ${chibios}/os/ports/GCC/PPC/SPC560Pxx/*.h            ./SPC560Pxx/ChibiOS-RT_SPC560Pxx_Kernel_Port_Component/component/lib/include/
cp -f ${chibios}/os/ports/GCC/PPC/*.c                      ./SPC560Pxx/ChibiOS-RT_SPC560Pxx_Kernel_Port_Component/component/lib/src/
cp -f ${chibios}/os/ports/GCC/PPC/ivor.s                   ./SPC560Pxx/ChibiOS-RT_SPC560Pxx_Kernel_Port_Component/component/lib/src/
cp -f ${chibios}/os/ports/GCC/PPC/SPC560Pxx/vectors.s      ./SPC560Pxx/ChibiOS-RT_SPC560Pxx_Kernel_Port_Component/component/lib/src/

#============================================================================#
# SPC563Mxx Components.                                                      #
#============================================================================#

# SPC563Mxx Kernel Port.
echo "SPC563Mxx Kernel Port"
cp -f ${chibios}/os/ports/GCC/PPC/*.h                      ./SPC563Mxx/ChibiOS-RT_SPC563Mxx_Kernel_Port_Component/component/lib/include/
cp -f ${chibios}/os/ports/GCC/PPC/SPC563Mxx/*.h            ./SPC563Mxx/ChibiOS-RT_SPC563Mxx_Kernel_Port_Component/component/lib/include/
cp -f ${chibios}/os/ports/GCC/PPC/*.c                      ./SPC563Mxx/ChibiOS-RT_SPC563Mxx_Kernel_Port_Component/component/lib/src/
cp -f ${chibios}/os/ports/GCC/PPC/ivor.s                   ./SPC563Mxx/ChibiOS-RT_SPC563Mxx_Kernel_Port_Component/component/lib/src/
cp -f ${chibios}/os/ports/GCC/PPC/SPC563Mxx/vectors.s      ./SPC563Mxx/ChibiOS-RT_SPC563Mxx_Kernel_Port_Component/component/lib/src/

#============================================================================#
# SPC564Axx Components.                                                      #
#============================================================================#

# SPC564Axx Kernel Port.
echo "SPC564Axx Kernel Port"
cp -f ${chibios}/os/ports/GCC/PPC/*.h                      ./SPC564Axx/ChibiOS-RT_SPC564Axx_Kernel_Port_Component/component/lib/include/
cp -f ${chibios}/os/ports/GCC/PPC/SPC564Axx/*.h            ./SPC564Axx/ChibiOS-RT_SPC564Axx_Kernel_Port_Component/component/lib/include/
cp -f ${chibios}/os/ports/GCC/PPC/*.c                      ./SPC564Axx/ChibiOS-RT_SPC564Axx_Kernel_Port_Component/component/lib/src/
cp -f ${chibios}/os/ports/GCC/PPC/ivor.s                   ./SPC564Axx/ChibiOS-RT_SPC564Axx_Kernel_Port_Component/component/lib/src/
cp -f ${chibios}/os/ports/GCC/PPC/SPC564Axx/vectors.s      ./SPC564Axx/ChibiOS-RT_SPC564Axx_Kernel_Port_Component/component/lib/src/

#============================================================================#
# SPC56ECxx Components.                                                      #
#============================================================================#

# SPC56ECxx Kernel Port.
echo "SPC56ECxx Kernel Port"
cp -f ${chibios}/os/ports/GCC/PPC/*.h                      ./SPC56ECxx/ChibiOS-RT_SPC56ECxx_Kernel_Port_Component/component/lib/include/
cp -f ${chibios}/os/ports/GCC/PPC/SPC56ECxx/*.h            ./SPC56ECxx/ChibiOS-RT_SPC56ECxx_Kernel_Port_Component/component/lib/include/
cp -f ${chibios}/os/ports/GCC/PPC/*.c                      ./SPC56ECxx/ChibiOS-RT_SPC56ECxx_Kernel_Port_Component/component/lib/src/
cp -f ${chibios}/os/ports/GCC/PPC/ivor.s                   ./SPC56ECxx/ChibiOS-RT_SPC56ECxx_Kernel_Port_Component/component/lib/src/
cp -f ${chibios}/os/ports/GCC/PPC/SPC56ECxx/vectors.s      ./SPC56ECxx/ChibiOS-RT_SPC56ECxx_Kernel_Port_Component/component/lib/src/

#============================================================================#
# SPC56ELxx Components.                                                      #
#============================================================================#

# SPC56ELxx Kernel Port.
echo "SPC56ELxx Kernel Port"
cp -f ${chibios}/os/ports/GCC/PPC/*.h                      ./SPC56ELxx/ChibiOS-RT_SPC56ELxx_Kernel_Port_Component/component/lib/include/
cp -f ${chibios}/os/ports/GCC/PPC/SPC56ELxx/*.h            ./SPC56ELxx/ChibiOS-RT_SPC56ELxx_Kernel_Port_Component/component/lib/include/
cp -f ${chibios}/os/ports/GCC/PPC/*.c                      ./SPC56ELxx/ChibiOS-RT_SPC56ELxx_Kernel_Port_Component/component/lib/src/
cp -f ${chibios}/os/ports/GCC/PPC/ivor.s                   ./SPC56ELxx/ChibiOS-RT_SPC56ELxx_Kernel_Port_Component/component/lib/src/
cp -f ${chibios}/os/ports/GCC/PPC/SPC56ELxx/vectors.s      ./SPC56ELxx/ChibiOS-RT_SPC56ELxx_Kernel_Port_Component/component/lib/src/

#============================================================================#
# STM32F4xx Components.                                                      #
#============================================================================#

# STM32F4xx Platform.
echo "STM32F4xx Platform"
cp -f ${chibios}/os/ports/GCC/ARMCMx/rules.mk              ./STM32F4xx/ChibiOS-RT_STM32F4xx_Platform_Component/component/lib/rsc/
cp -f ${chibios}/os/ports/GCC/ARMCMx/crt0.c                ./STM32F4xx/ChibiOS-RT_STM32F4xx_Platform_Component/component/lib/src/

# STM32F4xx Kernel Port.
echo "STM32F4xx Kernel Port"
cp -f ${chibios}/os/ports/common/ARMCMx/nvic.h             ./STM32F4xx/ChibiOS-RT_STM32F4xx_Kernel_Port_Component/component/lib/include/
cp -f ${chibios}/os/ports/common/ARMCMx/nvic.c             ./STM32F4xx/ChibiOS-RT_STM32F4xx_Kernel_Port_Component/component/lib/src/
cp -f ${chibios}/os/ports/GCC/ARMCMx/chcore.h              ./STM32F4xx/ChibiOS-RT_STM32F4xx_Kernel_Port_Component/component/lib/include/
cp -f ${chibios}/os/ports/GCC/ARMCMx/chcore_v7m.h          ./STM32F4xx/ChibiOS-RT_STM32F4xx_Kernel_Port_Component/component/lib/include/
cp -f ${chibios}/os/ports/GCC/ARMCMx/chtypes.h             ./STM32F4xx/ChibiOS-RT_STM32F4xx_Kernel_Port_Component/component/lib/include/
cp -f ${chibios}/os/ports/GCC/ARMCMx/STM32F4xx/*.h         ./STM32F4xx/ChibiOS-RT_STM32F4xx_Kernel_Port_Component/component/lib/include/
cp -f ${chibios}/os/ports/GCC/ARMCMx/chcore.c              ./STM32F4xx/ChibiOS-RT_STM32F4xx_Kernel_Port_Component/component/lib/src/
cp -f ${chibios}/os/ports/GCC/ARMCMx/chcore_v7m.c          ./STM32F4xx/ChibiOS-RT_STM32F4xx_Kernel_Port_Component/component/lib/src/
cp -f ${chibios}/os/ports/GCC/ARMCMx/STM32F4xx/*.c         ./STM32F4xx/ChibiOS-RT_STM32F4xx_Kernel_Port_Component/component/lib/src/

# STM32F4xx HAL Platform.
echo "STM32F4xx HAL Platform"
cp -f ${chibios}/os/hal/platforms/STM32/*.h                ./STM32F4xx/ChibiOS-RT_STM32F4xx_HAL_Platform_Component/component/lib/include/
cp -f ${chibios}/os/hal/platforms/STM32/*.c                ./STM32F4xx/ChibiOS-RT_STM32F4xx_HAL_Platform_Component/component/lib/src/
cp -f ${chibios}/os/hal/platforms/STM32/GPIOv2/*.h         ./STM32F4xx/ChibiOS-RT_STM32F4xx_HAL_Platform_Component/component/lib/include/
cp -f ${chibios}/os/hal/platforms/STM32/GPIOv2/*.c         ./STM32F4xx/ChibiOS-RT_STM32F4xx_HAL_Platform_Component/component/lib/src/
cp -f ${chibios}/os/hal/platforms/STM32/I2Cv1/*.h          ./STM32F4xx/ChibiOS-RT_STM32F4xx_HAL_Platform_Component/component/lib/include/
cp -f ${chibios}/os/hal/platforms/STM32/I2Cv1/*.c          ./STM32F4xx/ChibiOS-RT_STM32F4xx_HAL_Platform_Component/component/lib/src/
cp -f ${chibios}/os/hal/platforms/STM32/OTGv1/*.h          ./STM32F4xx/ChibiOS-RT_STM32F4xx_HAL_Platform_Component/component/lib/include/
cp -f ${chibios}/os/hal/platforms/STM32/OTGv1/*.c          ./STM32F4xx/ChibiOS-RT_STM32F4xx_HAL_Platform_Component/component/lib/src/
cp -f ${chibios}/os/hal/platforms/STM32/RTCv2/*.h          ./STM32F4xx/ChibiOS-RT_STM32F4xx_HAL_Platform_Component/component/lib/include/
cp -f ${chibios}/os/hal/platforms/STM32/RTCv2/*.c          ./STM32F4xx/ChibiOS-RT_STM32F4xx_HAL_Platform_Component/component/lib/src/
cp -f ${chibios}/os/hal/platforms/STM32/SPIv1/*.h          ./STM32F4xx/ChibiOS-RT_STM32F4xx_HAL_Platform_Component/component/lib/include/
cp -f ${chibios}/os/hal/platforms/STM32/SPIv1/*.c          ./STM32F4xx/ChibiOS-RT_STM32F4xx_HAL_Platform_Component/component/lib/src/
cp -f ${chibios}/os/hal/platforms/STM32/USARTv1/*.h        ./STM32F4xx/ChibiOS-RT_STM32F4xx_HAL_Platform_Component/component/lib/include/
cp -f ${chibios}/os/hal/platforms/STM32/USARTv1/*.c        ./STM32F4xx/ChibiOS-RT_STM32F4xx_HAL_Platform_Component/component/lib/src/
cp -f ${chibios}/os/hal/platforms/STM32F4xx/*.h            ./STM32F4xx/ChibiOS-RT_STM32F4xx_HAL_Platform_Component/component/lib/include/
cp -f ${chibios}/os/hal/platforms/STM32F4xx/*.c            ./STM32F4xx/ChibiOS-RT_STM32F4xx_HAL_Platform_Component/component/lib/src/

echo ""
echo "Done!"