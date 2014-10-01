*****************************************************************************
** ChibiOS/RT port for BCM2835 / ARM1176JZF-S
*****************************************************************************

** TARGET **

The I2C demo runs on an Raspberry Pi RevB board.

** The Demo **

This demo reads a TMP102 temperature sensor and uses a MCP4725 12 bit DAC
(Digital/Analog Converter) to generate a voltage representing the
fahrenheit temperature in millivolts. For example, a temperature of 75
degrees would generate 75 millivolts. Therefore, a digital voltmeter
can be used as a digital temperature display.

** Build Procedure **

This was built with the Yagarto GCC toolchain.

** Notes **

