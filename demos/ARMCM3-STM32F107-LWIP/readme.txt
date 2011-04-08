*****************************************************************************
** ChibiOS/RT port for ARM-Cortex-M3 STM32F107.                            **
*****************************************************************************

** TARGET **

The demo runs on an Olimex STM32-P107 board.

** The Demo **

The demo flashes the board LED using a thread, by pressing the button located
on the board the test procedure is activated with output on the serial port
SD3 (USART3).
The demo also starts the ethernet driver using the IP address configured in
lwipthread.h.

** Build Procedure **

The demo has been tested by using the free Codesourcery GCC-based toolchain.
Just modify the TRGT line in the makefile in order to use different GCC ports.
The demo requires the lwip 1.4.0.rc1 stack, which is included in ./ext and
should be extracted to ./ext/lwip
Additionally, the demo requires the ST firmware library, which is included in
./ext

** Notes **

Some files used by the demo are not part of ChibiOS/RT but are copyright of
ST Microelectronics and are licensed under a different license.
Also note that not all the files present in the ST library are distribited
with ChibiOS/RT, you can find the whole library on the ST web site:

                             http://www.st.com

The lwIP stack also has its own license, please read the info into the included
lwIP distribution files.
