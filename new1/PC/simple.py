from time import sleep
import numpy as np
import serial


if __name__ == "__main__":
    serial_psoc = serial.Serial(port='COM4', baudrate=115200, timeout=None)
    serial_psoc.reset_input_buffer()
    serial_psoc.read_until(expected=serial.LF)

    while True:
        data = serial_psoc.read_until(
            expected=serial.LF).decode("utf-8").split(',')[:-1]

        print(np.array(data).reshape(8, 8))
        #print(np.rot90(np.array(data).reshape(8, 8), 2))
        print("\033c", end='')
