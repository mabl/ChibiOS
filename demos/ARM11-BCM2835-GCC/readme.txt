*****************************************************************************
** ChibiOS/RT port for BCM2835 / ARM1176JZF-S
*****************************************************************************

** TARGET **

The demo runs on an Raspberry Pi RevB (BCM2835) board.

** The Demo **

One thread flashes the onboard LED. Another thread runs a simple
command shell. Type 'help' to see the commands in the shell.

** Build Procedure **

This demo was built using the Yagarto GCC toolchain. Just type 'make'
and the executable will be built to build/ch.bin. 

To enable additional shell commands, including a built-in test suite,
add -DEXTENDED_SHELL to the make command line when building the
code.

** Notes **

To use the shell, a serial interface to the Raspberry is required (a
3.3V USB to FTDI adapter, for example).

