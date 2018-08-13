#!/usr/bin/python3
import serial

fd = serial.Serial('/dev/ttyUSB0', 115200, parity=serial.PARITY_EVEN, timeout=10)

fd.write(b'0123456789abcdefg')

print(fd.read(10))

fd.close()
