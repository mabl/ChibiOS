del latex\*.* /Q
copy ..\common\rsc\chibios.sty .\latex
doxygen Doxyfile_pdf
cd latex
copy ..\..\common\rsc\Makefile .
mingw32-make all
copy refman.pdf ..\chibios_nil_1.0.0_kernel_rm.pdf
pause
