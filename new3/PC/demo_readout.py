import datetime
import time
import numpy as np
import serial

# Protocol
START_BYTE = 0x81
TYPE_GET_DATA_CONTINUOUS = 0x04
TYPE_SET_OFFSET = 0x05
TYPE_SET_STOP_DATA = 0x08
END_BYTE = 0x7E
RESPONSE_LENGTH_GET_DATA_CONTINUOUS = 135


def request_get_continuous_data(delay_switch, delay_meas, input_output):
    request = []
    request.append(START_BYTE)
    request.append(TYPE_GET_DATA_CONTINUOUS)
    request.append((delay_switch >> 8) & 0xFF)
    request.append(delay_switch & 0xFF)
    request.append((delay_meas >> 8) & 0xFF)
    request.append(delay_meas & 0xFF)
    for io in input_output:
        request.append(io)
    request.append(END_BYTE)

    return bytes(request)


def request_set_offset():
    request = []
    request.append(START_BYTE)
    request.append(TYPE_SET_OFFSET)
    request.append(END_BYTE)
    return bytes(request)


def request_stop_data():
    request = []
    request.append(START_BYTE)
    request.append(TYPE_SET_STOP_DATA)
    request.append(END_BYTE)
    return bytes(request)


if __name__ == "__main__":
    serial_psoc = serial.Serial(port='COM12', baudrate=115200, timeout=None)

    # Send request to get data continuously
    resistance_selection = [(0, 1), (0, 2), ]
    input_output = [((i << 4) & 0xFF) | (o & 0xFF)
                    for i, o in resistance_selection]
    delay_switch = 500
    delay_meas = 50
    request_get_continuous_data = request_get_continuous_data(
        delay_switch, delay_meas, input_output)  # request = 8104012c003201027e
    print(request_get_continuous_data.hex())
    serial_psoc.write(request_get_continuous_data)
    time.sleep(0.5)
    request_set_offset = request_set_offset()
    serial_psoc.write(request_set_offset)

    csv_path = datetime.datetime.now().strftime('%Y-%m-%d_%H_%M_%S') + '.csv'
    f = open(csv_path, 'w+')
    try:
        while True:
            response = serial_psoc.read(size=7+2*len(input_output))
            # print(response.hex())
            timestamp = np.frombuffer(
                response, dtype='>u4', count=1, offset=2)[0]
            sensor_data = np.frombuffer(response, dtype='>u2', count=len(
                input_output), offset=6)        # '>u2' is big-endian uint16
            print(f'timestamp: {timestamp}')
            print(sensor_data)

            print("\033c", end='')  # Refresh ternimal
            np.savetxt(
                f, [np.concatenate(([timestamp], sensor_data))], delimiter=',', fmt='%s')

    except KeyboardInterrupt:
        f.close()
        serial_psoc.write(request_stop_data())
        print('Keyboard Interrupt')
        print('Report saved to ', csv_path)
