del html\*.* /Q
doxygen Doxyfile_chm
del html\*.md5
del html\*.map
copy html\refman.chm .\chibios_rt_2.6.0_kernel_hal_rm.chm
pause
