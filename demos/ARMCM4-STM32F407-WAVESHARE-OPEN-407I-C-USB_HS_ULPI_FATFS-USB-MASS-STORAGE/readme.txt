*****************************************************************************
** ChibiOS/RT port for ARM-Cortex-M4 STM32F407.                            **
*****************************************************************************

** TARGET **

The demo runs on an WaveShare STM32F4 Open 407I-C board.

** The Demo **

This will demonstrate the use of a USB Mass Storage device over the high 
speed ULPI interface to access an SD card.

** Build Procedure **

Refer to demo_picture.jpg.

Make sure you do not plug anything into the other pin headers on the board.
Only connect the USB3300 board, the SD board, and optionally the ethernet 
board. Most of the other pin headers conflict with either the SDIO pins
or the ULPI pins. You can see debug output on pins PD5 and PD6 of the header.

The demo has been tested by using the free Codesourcery GCC-based toolchain. 
Just modify the TRGT line in the makefile in order to use different GCC 
toolchains.

** Notes **

Some files used by the demo are not part of ChibiOS/RT but are copyright of
ST Microelectronics and are licensed under a different license.
Also note that not all the files present in the ST library are distributed
with ChibiOS/RT, you can find the whole library on the ST web site:

                             http://www.st.com
