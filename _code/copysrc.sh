#!/bin/bash
code=.
chibios=../../../trunk
#do_templates=yes

#============================================================================#
# Portable Components.                                                       #
#============================================================================#

# Portable Kernel.
echo "Portable Kernel"
if [ ! -d $do_templates ]
then
  cp ${chibios}/os/nil/templates/nilconf.h                  ../portable/NIL_Portable_Kernel_Component/component/cfg/nilconf.h.ftl
fi
cp -R ${chibios}/os/nil/include                             ../portable/NIL_Portable_Kernel_Component/component/lib
cp -R ${chibios}/os/nil/src                                 ../portable/NIL_Portable_Kernel_Component/component/lib

# Portable OSAL.
echo "Portable OSAL"
cp ${chibios}/os/hal/osal/nil/*.h                           ../portable/NIL_Portable_OSAL_Component/component/lib/include
cp ${chibios}/os/hal/osal/nil/*.c                           ../portable/NIL_Portable_OSAL_Component/component/lib/src

#============================================================================#
# SPC560Dxx Components.                                                      #
#============================================================================#

# SPC560Dxx Kernel Port.
echo "SPC560Dxx Kernel Port"
cp ${code}/ports/e200/cfg/*.ftl                             ../SPC560Dxx/NIL_SPC560Dxx_Kernel_Port_Component/component/root_wa/source/isrs/
cp ${chibios}/os/nil/ports/e200/*.h                         ../SPC560Dxx/NIL_SPC560Dxx_Kernel_Port_Component/component/lib/include/
cp ${chibios}/os/nil/ports/e200/*.c                         ../SPC560Dxx/NIL_SPC560Dxx_Kernel_Port_Component/component/lib/src/
cp ${chibios}/os/nil/ports/e200/compilers/GCC/*.h           ../SPC560Dxx/NIL_SPC560Dxx_Kernel_Port_Component/component/lib/include/
cp ${chibios}/os/nil/ports/e200/compilers/GCC/ivor.s        ../SPC560Dxx/NIL_SPC560Dxx_Kernel_Port_Component/component/lib/src/
cp ${chibios}/os/common/ports/e200/compilers/GCC/*.h        ../SPC560Dxx/NIL_SPC560Dxx_Kernel_Port_Component/component/lib/include/
cp ${chibios}/os/common/ports/e200/compilers/GCC/vectors.s  ../SPC560Dxx/NIL_SPC560Dxx_Kernel_Port_Component/component/lib/src/
cp ${chibios}/os/common/ports/e200/devices/SPC560Dxx/intc.h ../SPC560Dxx/NIL_SPC560Dxx_Kernel_Port_Component/component/lib/include/
cp ${chibios}/os/common/ports/e200/devices/SPC560Dxx/ppcparams.h ../SPC560Dxx/NIL_SPC560Dxx_Kernel_Port_Component/component/lib/include/


#============================================================================#
# SPC560Bxx Components.                                                      #
#============================================================================#

# SPC560Bxx Kernel Port.
echo "SPC560Bxx Kernel Port"
cp ${code}/ports/e200/cfg/*.ftl                             ../SPC560Bxx/NIL_SPC560Bxx_Kernel_Port_Component/component/root_wa/source/isrs/
cp ${chibios}/os/nil/ports/e200/*.h                         ../SPC560Bxx/NIL_SPC560Bxx_Kernel_Port_Component/component/lib/include/
cp ${chibios}/os/nil/ports/e200/*.c                         ../SPC560Bxx/NIL_SPC560Bxx_Kernel_Port_Component/component/lib/src/
cp ${chibios}/os/nil/ports/e200/compilers/GCC/*.h           ../SPC560Bxx/NIL_SPC560Bxx_Kernel_Port_Component/component/lib/include/
cp ${chibios}/os/nil/ports/e200/compilers/GCC/ivor.s        ../SPC560Bxx/NIL_SPC560Bxx_Kernel_Port_Component/component/lib/src/
cp ${chibios}/os/common/ports/e200/compilers/GCC/*.h        ../SPC560Bxx/NIL_SPC560Bxx_Kernel_Port_Component/component/lib/include/
cp ${chibios}/os/common/ports/e200/compilers/GCC/vectors.s  ../SPC560Bxx/NIL_SPC560Bxx_Kernel_Port_Component/component/lib/src/
cp ${chibios}/os/common/ports/e200/devices/SPC560Bxx/intc.h ../SPC560Bxx/NIL_SPC560Bxx_Kernel_Port_Component/component/lib/include/
cp ${chibios}/os/common/ports/e200/devices/SPC560Bxx/ppcparams.h ../SPC560Bxx/NIL_SPC560Bxx_Kernel_Port_Component/component/lib/include/

#============================================================================#
# SPC57EMxx HSM Components.                                                  #
#============================================================================#

# SPC57EMxx HSM Kernel Port.
echo "SPC57EMxx HSM Kernel Port"
cp ${code}/ports/e200/cfg/*.ftl                             ../SPC57EMxx_HSM/NIL_SPC57EMxx_HSM_Kernel_Port_Component/component/root_wa/source/isrs/
cp ${chibios}/os/nil/ports/e200/*.h                         ../SPC57EMxx_HSM/NIL_SPC57EMxx_HSM_Kernel_Port_Component/component/lib/include/
cp ${chibios}/os/nil/ports/e200/*.c                         ../SPC57EMxx_HSM/NIL_SPC57EMxx_HSM_Kernel_Port_Component/component/lib/src/
cp ${chibios}/os/nil/ports/e200/compilers/GCC/*.h           ../SPC57EMxx_HSM/NIL_SPC57EMxx_HSM_Kernel_Port_Component/component/lib/include/
cp ${chibios}/os/nil/ports/e200/compilers/GCC/ivor.s        ../SPC57EMxx_HSM/NIL_SPC57EMxx_HSM_Kernel_Port_Component/component/lib/src/
cp ${chibios}/os/common/ports/e200/compilers/GCC/*.h        ../SPC57EMxx_HSM/NIL_SPC57EMxx_HSM_Kernel_Port_Component/component/lib/include/
cp ${chibios}/os/common/ports/e200/compilers/GCC/vectors.s  ../SPC57EMxx_HSM/NIL_SPC57EMxx_HSM_Kernel_Port_Component/component/lib/src/
cp ${chibios}/os/common/ports/e200/devices/SPC57EMxx_HSM/intc.h ../SPC57EMxx_HSM/NIL_SPC57EMxx_HSM_Kernel_Port_Component/component/lib/include/
cp ${chibios}/os/common/ports/e200/devices/SPC57EMxx_HSM/ppcparams.h ../SPC57EMxx_HSM/NIL_SPC57EMxx_HSM_Kernel_Port_Component/component/lib/include/

echo ""
echo "Done!"
