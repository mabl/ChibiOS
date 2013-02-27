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

The following write performance tests were performed Feb 26, 2013, using the Wave Share 407I-C Dev board on the USB High Speed interface.
  * A-Data, 8gig SD card, Class 6, 3.0megabytes/second write speed
  * Kingston, 32gig SD card, Class 10, 3.9megabytes/second write speed
  * Kingston, 16 gig SD card, Class 4, 2.9megabytes/second write speed
  * RiData, 8 gig SD card, Class 6, 2.4megabytes/second write speed
  * Sandisk, 32 gig SD card, Class 4, 3.9megabytes/second write speed
  * Sandisk Ultra, 16gig SD card, HCI, 3.1megabytes/second write speed
  * Sandisk, 8 gig SD card, Class 4, 3.9megabytes/second write speed
  * Sandisk, 4 gig SD card, Class 6, 3.9megabytes/second write speed
  * Transend, 16 gig SD card, Class 10, 3.9megabytes/second write speed
  * Transend, 8 gig SD card, Class 6, 2.8megabytes/second write speed
  

Some files used by the demo are not part of ChibiOS/RT but are copyright of
ST Microelectronics and are licensed under a different license.
Also note that not all the files present in the ST library are distributed
with ChibiOS/RT, you can find the whole library on the ST web site:

                             http://www.st.com
