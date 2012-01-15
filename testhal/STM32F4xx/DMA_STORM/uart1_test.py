#!/usr/bin/python
# -*- coding: cp1251 -*-

import serial

baudrate = 115200
port     = 0

print "opening port..."
ser = serial.Serial(port, baudrate)

print "begin writing to port..."
i = 0
n = 0
while True:
    ser.write('U')
    i += 1
    n += 1
    if n == 1024:
        print i/1024, "kbytes sent"
        n = 0
