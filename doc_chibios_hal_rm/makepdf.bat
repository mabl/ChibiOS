del latex\*.* /Q
copy ..\common\rsc\chibios.sty .\latex
doxygen Doxyfile_pdf
cd latex
copy ..\..\common\rsc\Makefile .
mingw32-make all
copy refman.pdf ..\chibios_hal_3.0.0_rm.pdf
pause
