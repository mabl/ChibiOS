This demo applications runs on a SPC56x evaluation board with a SPC560B
mini-module. The LED jumpers must be inserted and the serial connector
configured for RS232 mode.

The demo starts a thread showing flashing LEDs and another thread running
a command shell over LINFLEX1, you can use a terminal emulator in order to
five commands to the application, the communication parameters must be
programmed as 38400-8-N-1. Type "help" for a list of available commands.
