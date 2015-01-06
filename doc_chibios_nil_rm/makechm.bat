del html\*.* /Q
doxygen Doxyfile_chm
del html\*.md5
del html\*.map
copy html\refman.chm .\chibios_nil_1.0.0_kernel_rm.chm
pause
