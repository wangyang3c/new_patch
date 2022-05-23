from plot import *
from PyQt6.QtCore import QThread, pyqtSignal, QUrl
import serial


class DataReceiver(QThread):
    update_plot = pyqtSignal(np.ndarray)


    def __init__(self, serial):
        super().__init__()
        self.serial = serial

    def run(self):
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
        serial_psoc.write(request)
    
        while True:
            if self.serial.in_waiting:
                response = serial_psoc.read(size=135)
                timestamp = np.frombuffer(response, dtype='>u4', count=1, offset=2)[0]
                data = ((np.frombuffer(response, dtype='>u2', count=64, offset=6) + 0x8000)*(20/65536)-10).reshape(8, 8)[:4,:4] 
                self.update_plot.emit(data)

if __name__ == '__main__':
    app = QtGui.QApplication(sys.argv)

    serial_psoc = serial.Serial(port='COM4', baudrate=115200, timeout=None)
    plot = BarPlot()

    receive_data_thread = DataReceiver(serial_psoc)
    receive_data_thread.update_plot.connect(plot.update_h)
    receive_data_thread.start()

    timer = QtCore.QTimer()
    timer.timeout.connect(plot.update_plot)
    timer.start(10)

    app.exec()