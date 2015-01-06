del latex\*.* /Q
copy ..\common\rsc\chibios.sty .\latex
doxygen Doxyfile_pdf
cd latex
copy ..\..\common\rsc\Makefile .
mingw32-make all
copy refman.pdf ..\chibios_rt_2.6.0_kernel_hal_rm.pdf
pause
