*****************************************************************************
** ChibiOS/RT port for ColdFire MCF5206e.                                  **
*****************************************************************************

** TARGET **

The demo runs on an my board.

** The Demo **

The demo currently just flashes the LED using a thread, prints an "Hello World!"
string on COM1 and activates che ChibiOS/RT test suite, output on COM1 periodically.

** Build Procedure **

The demo was built using the Sourcery G++ toolchain (http://www.codesourcery.com
) but any toolchain based on GCC
and GNU userspace programs will work.

** Notes **

The demo requires include file from CFInit that are not part of the ChibiOS/RT
distribution, please install CFInit (http://www.microapl.co.uk/CFInit/cfinit_download.html).
