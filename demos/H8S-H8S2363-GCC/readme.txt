*****************************************************************************
** ChibiOS/RT port for H8S2363.                                            **
*****************************************************************************

** TARGET **

The demo runs on an my board.
FLASH:  2MB mapped after reset at 0x200000
SRAM:   ~2MB from 0x011000
Area 0x000000 to 0x011000 is reserved for monitor and bootloader.


** The Demo **

The demo currently just flashes the LED using a thread, prints an "Hello World!"
string on COM1 and activates che ChibiOS/RT test suite output on COM1, periodically.

** Build Procedure **

The demo was built using the 4.4-GNUH8_v10.01 toolchain from KPIT Cummins Infosystems Limited
(http://www.kpitgnutools.com/).

** Notes **

