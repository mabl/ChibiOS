#!/bin/bash
chibios=/cygdrive/d/Progetti/ChibiOS-RT
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
