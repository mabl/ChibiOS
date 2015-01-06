del html\*.* /Q
doxygen Doxyfile_chm
del html\*.md5
del html\*.map
copy html\refman.chm .\chibios_hal_3.0.0_rm.chm
pause
