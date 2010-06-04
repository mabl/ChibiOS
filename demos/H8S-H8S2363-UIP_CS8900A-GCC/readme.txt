*****************************************************************************
** ChibiOS/RT port for H8S2363 with CS8900A.                               **
*****************************************************************************

** TARGET **

The demo runs on an my board.
FLASH:  2MB mapped after reset at 0x200000
SRAM:   ~2MB from 0x011000
Area 0x000000 to 0x011000 is reserved for monitor and bootloader.
Crystal CS8900A as an ethernet controller (I/O mode only).

** The Demo **

The demo currently serves HTTP requests at address 10.0.0.2 on port 80
(remember to change its IP address into webthread.c in order to adapt it
to your network settings). Demo also prints a "Hello World!" string on COM1
and activates the ChibiOS/RT test suite output on COM1 in background.

** Build Procedure **

The demo was built using the 4.4-GNUH8_v10.01 toolchain
from KPIT Cummins Infosystems Limited (http://www.kpitgnutools.com/).
The demo requires the patcher uIP 1.0 stack, see: ./ext/readme.txt

** Notes **

The uIP stack has its own license, please read the info into the included
uIP distribution files.
