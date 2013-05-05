#!/bin/bash
#chibios=/cygdrive/d/Progetti/ChibiOS-RT
chibios=/cygdrive/d/Progetti/ChibiOS-RT-stable-2.6.x
#chibios=../../chibios
#do_templates=yes

#============================================================================#
# Portable Components.                                                       #
#============================================================================#

# Portable Kernel.
if [ ! -d $do_templates ]
then
  cp ${chibios}/os/kernel/templates/chconf.h ./portable/ChibiOS-RT_Portable_Kernel_Component/component/cfg/chconf.h.ftl
fi
cp -R ${chibios}/os/kernel/include ./portable/ChibiOS-RT_Portable_Kernel_Component/component/lib
cp -R ${chibios}/os/kernel/src ./portable/ChibiOS-RT_Portable_Kernel_Component/component/lib

# Portable HAL.
if [ ! -d $do_templates ]
then
  cp ${chibios}/os/hal/templates/halconf.h ./portable/ChibiOS-RT_Portable_HAL_Component/component/cfg/halconf.h.ftl
fi
cp -R ${chibios}/os/hal/include ./portable/ChibiOS-RT_Portable_HAL_Component/component/lib
cp -R ${chibios}/os/hal/src ./portable/ChibiOS-RT_Portable_HAL_Component/component/lib

# Portable Kernel Test Suite.
cp ${chibios}/test/*.h ./portable/ChibiOS-RT_Portable_Kernel_Test_Suite_Component/component/lib/include/
cp ${chibios}/test/*.c ./portable/ChibiOS-RT_Portable_Kernel_Test_Suite_Component/component/lib/src/

# Portable Output Formatter.
cp ${chibios}/os/various/chprintf.h ./portable/ChibiOS-RT_Portable_Output_Formatter_Component/component/lib/include/
cp ${chibios}/os/various/chprintf.c ./portable/ChibiOS-RT_Portable_Output_Formatter_Component/component/lib/src/

# Portable Shell.
cp ${chibios}/os/various/shell.h ./portable/ChibiOS-RT_Portable_Mini_Shell_Component/component/lib/include/
cp ${chibios}/os/various/shell.c ./portable/ChibiOS-RT_Portable_Mini_Shell_Component/component/lib/src/

#============================================================================#
# SPC560BCxx Components.                                                     #
#============================================================================#

# SPC560BCxx Kernel Port.
echo "SPC560BCxx Kernel Port"
cp ${chibios}/os/ports/GCC/PPC/*.h                      ./SPC560BCxx/ChibiOS-RT_SPC560BCxx_Kernel_Port_Component/component/lib/include/
cp ${chibios}/os/ports/GCC/PPC/SPC560BCxx/*.h           ./SPC560BCxx/ChibiOS-RT_SPC560BCxx_Kernel_Port_Component/component/lib/include/
cp ${chibios}/os/ports/GCC/PPC/*.c                      ./SPC560BCxx/ChibiOS-RT_SPC560BCxx_Kernel_Port_Component/component/lib/src/
cp ${chibios}/os/ports/GCC/PPC/ivor.s                   ./SPC560BCxx/ChibiOS-RT_SPC560BCxx_Kernel_Port_Component/component/lib/src/
cp ${chibios}/os/ports/GCC/PPC/SPC560BCxx/vectors.s     ./SPC560BCxx/ChibiOS-RT_SPC560BCxx_Kernel_Port_Component/component/lib/src/

#============================================================================#
# SPC560Pxx Components.                                                      #
#============================================================================#

# SPC560Pxx Kernel Port.
echo "SPC560Pxx Kernel Port"
cp ${chibios}/os/ports/GCC/PPC/*.h                      ./SPC560Pxx/ChibiOS-RT_SPC560Pxx_Kernel_Port_Component/component/lib/include/
cp ${chibios}/os/ports/GCC/PPC/SPC560Pxx/*.h            ./SPC560Pxx/ChibiOS-RT_SPC560Pxx_Kernel_Port_Component/component/lib/include/
cp ${chibios}/os/ports/GCC/PPC/*.c                      ./SPC560Pxx/ChibiOS-RT_SPC560Pxx_Kernel_Port_Component/component/lib/src/
cp ${chibios}/os/ports/GCC/PPC/ivor.s                   ./SPC560Pxx/ChibiOS-RT_SPC560Pxx_Kernel_Port_Component/component/lib/src/
cp ${chibios}/os/ports/GCC/PPC/SPC560Pxx/vectors.s      ./SPC560Pxx/ChibiOS-RT_SPC560Pxx_Kernel_Port_Component/component/lib/src/

#============================================================================#
# SPC563Mxx Components.                                                      #
#============================================================================#

# SPC563Mxx Kernel Port.
echo "SPC563Mxx Kernel Port"
cp ${chibios}/os/ports/GCC/PPC/*.h                      ./SPC563Mxx/ChibiOS-RT_SPC563Mxx_Kernel_Port_Component/component/lib/include/
cp ${chibios}/os/ports/GCC/PPC/SPC563Mxx/*.h            ./SPC563Mxx/ChibiOS-RT_SPC563Mxx_Kernel_Port_Component/component/lib/include/
cp ${chibios}/os/ports/GCC/PPC/*.c                      ./SPC563Mxx/ChibiOS-RT_SPC563Mxx_Kernel_Port_Component/component/lib/src/
cp ${chibios}/os/ports/GCC/PPC/ivor.s                   ./SPC563Mxx/ChibiOS-RT_SPC563Mxx_Kernel_Port_Component/component/lib/src/
cp ${chibios}/os/ports/GCC/PPC/SPC563Mxx/vectors.s      ./SPC563Mxx/ChibiOS-RT_SPC563Mxx_Kernel_Port_Component/component/lib/src/

#============================================================================#
# SPC564Axx Components.                                                      #
#============================================================================#

# SPC564Axx Kernel Port.
echo "SPC564Axx Kernel Port"
cp ${chibios}/os/ports/GCC/PPC/*.h                      ./SPC564Axx/ChibiOS-RT_SPC564Axx_Kernel_Port_Component/component/lib/include/
cp ${chibios}/os/ports/GCC/PPC/SPC564Axx/*.h            ./SPC564Axx/ChibiOS-RT_SPC564Axx_Kernel_Port_Component/component/lib/include/
cp ${chibios}/os/ports/GCC/PPC/*.c                      ./SPC564Axx/ChibiOS-RT_SPC564Axx_Kernel_Port_Component/component/lib/src/
cp ${chibios}/os/ports/GCC/PPC/ivor.s                   ./SPC564Axx/ChibiOS-RT_SPC564Axx_Kernel_Port_Component/component/lib/src/
cp ${chibios}/os/ports/GCC/PPC/SPC564Axx/vectors.s      ./SPC564Axx/ChibiOS-RT_SPC564Axx_Kernel_Port_Component/component/lib/src/

#============================================================================#
# SPC56ELxx Components.                                                      #
#============================================================================#

# SPC56ELxx Kernel Port.
echo "SPC56ELxx Kernel Port"
cp ${chibios}/os/ports/GCC/PPC/*.h                      ./SPC56ELxx/ChibiOS-RT_SPC56ELxx_Kernel_Port_Component/component/lib/include/
cp ${chibios}/os/ports/GCC/PPC/SPC56ELxx/*.h            ./SPC56ELxx/ChibiOS-RT_SPC56ELxx_Kernel_Port_Component/component/lib/include/
cp ${chibios}/os/ports/GCC/PPC/*.c                      ./SPC56ELxx/ChibiOS-RT_SPC56ELxx_Kernel_Port_Component/component/lib/src/
cp ${chibios}/os/ports/GCC/PPC/ivor.s                   ./SPC56ELxx/ChibiOS-RT_SPC56ELxx_Kernel_Port_Component/component/lib/src/
cp ${chibios}/os/ports/GCC/PPC/SPC56ELxx/vectors.s      ./SPC56ELxx/ChibiOS-RT_SPC56ELxx_Kernel_Port_Component/component/lib/src/

#============================================================================#
# STM32F4xx Components.                                                      #
#============================================================================#

# STM32F4xx Kernel Port.
echo "STM32F4xx Kernel Port"
cp ${chibios}/os/ports/common/ARMCMx/nvic.h             ./STM32F4xx/ChibiOS-RT_STM32F4xx_Kernel_Port_Component/component/lib/include/
cp ${chibios}/os/ports/common/ARMCMx/nvic.c             ./STM32F4xx/ChibiOS-RT_STM32F4xx_Kernel_Port_Component/component/lib/src/
cp ${chibios}/os/ports/GCC/ARMCMx/chcore.h              ./STM32F4xx/ChibiOS-RT_STM32F4xx_Kernel_Port_Component/component/lib/include/
cp ${chibios}/os/ports/GCC/ARMCMx/chcore_v7m.h          ./STM32F4xx/ChibiOS-RT_STM32F4xx_Kernel_Port_Component/component/lib/include/
cp ${chibios}/os/ports/GCC/ARMCMx/chtypes.h.h           ./STM32F4xx/ChibiOS-RT_STM32F4xx_Kernel_Port_Component/component/lib/include/
cp ${chibios}/os/ports/GCC/ARMCMx/STM32F4xx/*.h         ./STM32F4xx/ChibiOS-RT_STM32F4xx_Kernel_Port_Component/component/lib/include/
cp ${chibios}/os/ports/GCC/ARMCMx/chcore.c              ./STM32F4xx/ChibiOS-RT_STM32F4xx_Kernel_Port_Component/component/lib/src/
cp ${chibios}/os/ports/GCC/ARMCMx/chcore_v7m.c          ./STM32F4xx/ChibiOS-RT_STM32F4xx_Kernel_Port_Component/component/lib/src/
cp ${chibios}/os/ports/GCC/ARMCMx/STM32F4xx/*.c         ./STM32F4xx/ChibiOS-RT_STM32F4xx_Kernel_Port_Component/component/lib/src/

# STM32F4xx HAL Platform.
echo "STM32F4xx HAL Platform"
cp ${chibios}/os/hal/platforms/STM32F4xx/*.h            ./STM32F4xx/ChibiOS-RT_STM32F4xx_HAL_Platform_Component/component/lib/include/
cp ${chibios}/os/hal/platforms/STM32F4xx/*.c            ./STM32F4xx/ChibiOS-RT_STM32F4xx_HAL_Platform_Component/component/lib/src/
