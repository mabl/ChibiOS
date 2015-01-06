@echo off
cd doc_kernel_hal_rm
cmd /C makehtml.bat
cd ..\doc_port_arm79_gcc_rm
cmd /C makehtml.bat
cd ..\doc_port_armcmx_gcc_rm
cmd /C makehtml.bat
cd ..\doc_port_armcmx_iar_rm
cmd /C makehtml.bat
cd ..\doc_port_armcmx_rvct_rm
cmd /C makehtml.bat
cd ..\doc_port_ppc_gcc_rm
cmd /C makehtml.bat
cd ..
pause
