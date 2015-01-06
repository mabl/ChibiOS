del html\*.* /Q
doxygen Doxyfile_chm
del html\*.md5
del html\*.map
copy html\refman.chm .\chibios_rt_3.0.0_kernel_rm.chm
pause
