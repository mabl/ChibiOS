@echo Building RT Kernel RM
@del html\*.* /Q 2>nul >nul
@doxygen Doxyfile_html
@del html\*.md5 2>nul >nul
@del html\*.map 2>nul >nul
