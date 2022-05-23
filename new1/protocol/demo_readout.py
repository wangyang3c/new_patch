import numpy as np
import serial

# Protocol 
START_BYTE = 0x81
TYPE_GET_DATA_CONTINUOUS = 0x04
END_BYTE = 0x7E
RESPONSE_LENGTH_GET_DATA_CONTINUOUS = 135

# Electronics
NUM_SENSOR_Y = 8
NUM_SENSOR_X = 8

if __name__ == "__main__":
    serial_psoc = serial.Serial(port='COM4', baudrate=115200, timeout=None)
    
    # Send request to get data continuously
    start = START_BYTE
    type = TYPE_GET_DATA_CONTINUOUS
    xmin = 0
    xmax = 3
    ymin = 0
    ymax = 3
    delay_switch = 300
    delay_meas = 50
    end = END_BYTE
    request = bytes([start,
                     type,
                     xmin, 
                     xmax, 
                     ymin, 
                     ymax,
                     (delay_switch >> 8) & 0xFF, 
                     delay_switch & 0xFF,
                     (delay_meas >> 8) & 0xFF, 
                     delay_meas & 0xFF,
                     end])
    serial_psoc.write(request)  # request = b'\x81\x04\x00\x07\x00\x07\x01\x2C\x00\x0A\x7E'


    while True:
        response = serial_psoc.read(size=RESPONSE_LENGTH_GET_DATA_CONTINUOUS)
        timestamp = np.frombuffer(response, dtype='>u4', count=1, offset=2)[0]
        #data = ((np.frombuffer(response, dtype='>u2', count=64, offset=6) + 0x8000)*(20/65536)-10).reshape(8, 8)  # '>u2' is big-endian uint16
        sensor_data = np.frombuffer(response, dtype='>u2', count=64, offset=6).reshape(NUM_SENSOR_Y, NUM_SENSOR_X)   # '>u2' is big-endian uint16
        print(f'timestamp: {timestamp}')
        print(sensor_data)
        print("\033c", end='')  # Refresh ternimal
