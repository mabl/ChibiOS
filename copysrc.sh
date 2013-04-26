#!/bin/bash
#chibios=/cygdrive/d/Progetti/ChibiOS-RT
chibios=../../chibios
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
cp ${chibios}/os/ports/GCC/PPC/*.h                      ./SPC560BCxx/ChibiOS-RT_SPC560BCxx_Kernel_Port_Component/component/lib/include/
cp ${chibios}/os/ports/GCC/PPC/SPC560BCxx/*.h           ./SPC560BCxx/ChibiOS-RT_SPC560BCxx_Kernel_Port_Component/component/lib/include/
cp ${chibios}/os/ports/GCC/PPC/*.c                      ./SPC560BCxx/ChibiOS-RT_SPC560BCxx_Kernel_Port_Component/component/lib/src/
cp ${chibios}/os/ports/GCC/PPC/ivor.s                   ./SPC560BCxx/ChibiOS-RT_SPC560BCxx_Kernel_Port_Component/component/lib/src/
cp ${chibios}/os/ports/GCC/PPC/SPC560BCxx/vectors.s     ./SPC560BCxx/ChibiOS-RT_SPC560BCxx_Kernel_Port_Component/component/lib/src/

#============================================================================#
# SPC560Pxx Components.                                                      #
#============================================================================#

# SPC560Pxx Kernel Port.
cp ${chibios}/os/ports/GCC/PPC/*.h                      ./SPC560Pxx/ChibiOS-RT_SPC560Pxx_Kernel_Port_Component/component/lib/include/
cp ${chibios}/os/ports/GCC/PPC/SPC560Pxx/*.h            ./SPC560Pxx/ChibiOS-RT_SPC560Pxx_Kernel_Port_Component/component/lib/include/
cp ${chibios}/os/ports/GCC/PPC/*.c                      ./SPC560Pxx/ChibiOS-RT_SPC560Pxx_Kernel_Port_Component/component/lib/src/
cp ${chibios}/os/ports/GCC/PPC/ivor.s                   ./SPC560Pxx/ChibiOS-RT_SPC560Pxx_Kernel_Port_Component/component/lib/src/
cp ${chibios}/os/ports/GCC/PPC/SPC560Pxx/vectors.s      ./SPC560Pxx/ChibiOS-RT_SPC560Pxx_Kernel_Port_Component/component/lib/src/

#============================================================================#
# SPC563Mxx Components.                                                      #
#============================================================================#

# SPC563Mxx Kernel Port.
cp ${chibios}/os/ports/GCC/PPC/*.h                      ./SPC563Mxx/ChibiOS-RT_SPC563Mxx_Kernel_Port_Component/component/lib/include/
cp ${chibios}/os/ports/GCC/PPC/SPC563Mxx/*.h            ./SPC563Mxx/ChibiOS-RT_SPC563Mxx_Kernel_Port_Component/component/lib/include/
cp ${chibios}/os/ports/GCC/PPC/*.c                      ./SPC563Mxx/ChibiOS-RT_SPC563Mxx_Kernel_Port_Component/component/lib/src/
cp ${chibios}/os/ports/GCC/PPC/ivor.s                   ./SPC563Mxx/ChibiOS-RT_SPC563Mxx_Kernel_Port_Component/component/lib/src/
cp ${chibios}/os/ports/GCC/PPC/SPC563Mxx/vectors.s      ./SPC563Mxx/ChibiOS-RT_SPC563Mxx_Kernel_Port_Component/component/lib/src/

#============================================================================#
# SPC56ELxx Components.                                                      #
#============================================================================#

# SPC56ELxx Kernel Port.
cp ${chibios}/os/ports/GCC/PPC/*.h                      ./SPC56ELxx/ChibiOS-RT_SPC56ELxx_Kernel_Port_Component/component/lib/include/
cp ${chibios}/os/ports/GCC/PPC/SPC56ELxx/*.h            ./SPC56ELxx/ChibiOS-RT_SPC56ELxx_Kernel_Port_Component/component/lib/include/
cp ${chibios}/os/ports/GCC/PPC/*.c                      ./SPC56ELxx/ChibiOS-RT_SPC56ELxx_Kernel_Port_Component/component/lib/src/
cp ${chibios}/os/ports/GCC/PPC/ivor.s                   ./SPC56ELxx/ChibiOS-RT_SPC56ELxx_Kernel_Port_Component/component/lib/src/
cp ${chibios}/os/ports/GCC/PPC/SPC56ELxx/vectors.s      ./SPC56ELxx/ChibiOS-RT_SPC56ELxx_Kernel_Port_Component/component/lib/src/
