from plot import *
from PyQt6.QtCore import QThread, pyqtSignal, QUrl
import serial


class DataReceiver(QThread):
    update_plot = pyqtSignal(np.ndarray)


    def __init__(self, serial):
        super().__init__()
        self.serial = serial

    def run(self):
        self.serial.reset_input_buffer()
        self.serial.read_until(expected=serial.LF)
        init_v = None
        while True:
            if self.serial.in_waiting:
                data = self.serial.read_until(
                expected=serial.LF).decode("utf-8").split(',')[:-1]
                data = np.array(data).astype(float).reshape(8,8)[:4,:4]
                if init_v is None:
                    init_v = data
                #print(data)
                self.update_plot.emit((init_v-data)*10)

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