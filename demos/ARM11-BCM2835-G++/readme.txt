*****************************************************************************
** ChibiOS/RT port for BCM2835 using G++.                                          ** 
*****************************************************************************

** TARGET **

The demo runs some graphics tests using the Adafruit 2.2" 18-bit color 
TFT LCD display. http://www.adafruit.com/products/797

** The Demo **

The demonstration uses modified versions of the Adafruit_GFX and
Adafruit_HX8340B classes. These were originally written for the Arduino.
The Print and Printable support classes are also modified versions of
the Arduino classes. The graphics test is based on the test provided
with the Adafruit libraries, modified to run on the Pi.

** Build Procedure **

The demo was built using the YAGARTO toolchain but any toolchain based on GCC
and GNU userspace programs will work.
