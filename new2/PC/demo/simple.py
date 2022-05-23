from sqlite3 import Timestamp
from time import sleep
import numpy as np
import serial
import os

if __name__ == "__main__":
    serial_psoc = serial.Serial(port='COM4', baudrate=115200, timeout=None)

    start = 0x81
    type = 0x04
    xmin = 0
    xmax = 3
    ymin = 0
    ymax = 3
    delay_switch = 300
    delay_meas = 50
    end = 0x7E

    request = bytes([start,
                     type,
                     xmin, xmax, ymin, ymax,
                     (delay_switch >> 8) & 0xFF, delay_switch & 0xFF,
                     (delay_meas >> 8) & 0xFF, delay_meas & 0xFF,
                     end])

    # print(request.hex())
    # request = b'\x81\x04\x00\x07\x00\x07\x01\x2C\x00\x0A\x7E'

    serial_psoc.write(request)

    while True:
        response = serial_psoc.read(size=135)
        timestamp = np.frombuffer(response, dtype='>u4', count=1, offset=2)[0]
        #data = ((np.frombuffer(response, dtype='>u2', count=64, offset=6) + 0x8000)*(20/65536)-10).reshape(8, 8)  # '>u2' is big-endian uint16
        data = np.frombuffer(response, dtype='>u2', count=64, offset=6).reshape(8, 8)   # '>u2' is big-endian uint16
        print(timestamp)
        print(data)
        print("\033c", end='')
