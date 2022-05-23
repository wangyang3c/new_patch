# -*- coding: utf-8 -*-
from pyqtgraph.Qt import QtCore, QtGui
import pyqtgraph.opengl as gl
import pyqtgraph as pg
import numpy as np
import sys


class MyBarItem(gl.GLMeshItem):
    def __init__(self, x, y, z, width, heigth):

        x = np.array(x).flatten()
        y = np.array(y).flatten()
        z = np.array(z).flatten()

        pos = np.array(list(zip(x, y, z)))

        width = np.array(width).flatten()
        heigth = np.array(heigth).flatten()

        size = np.array(list(zip(width, width, heigth)))



        nCubes = np.prod(pos.shape[:-1])
        cubeVerts = np.mgrid[0:2, 0:2, 0:2].reshape(
            3, 8).transpose().reshape(1, 8, 3)
        cubeFaces = np.array([
            [0, 1, 2], [3, 2, 1],
            [4, 5, 6], [7, 6, 5],
            [0, 1, 4], [5, 4, 1],
            [2, 3, 6], [7, 6, 3],
            [0, 2, 4], [6, 4, 2],
            [1, 3, 5], [7, 5, 3]]).reshape(1, 12, 3)
        size = size.reshape((nCubes, 1, 3))
        pos = pos.reshape((nCubes, 1, 3))
        verts = cubeVerts * size + pos
        faces = cubeFaces + (np.arange(nCubes) * 8).reshape(nCubes, 1, 1)

        cm = pg.colormap.get('CET-R2')
        colors = cm.map(heigth/3.3, mode=pg.ColorMap.FLOAT).reshape((nCubes,1,4))
        vertex_colors = np.tile(colors, (1, 8, 1))
        md = gl.MeshData(vertexes=verts.reshape(nCubes*8, 3),
                         faces=faces.reshape(nCubes*12, 3),
                         vertexColors=vertex_colors.reshape(nCubes*8, 4))

        gl.GLMeshItem.__init__(
            self, meshdata=md, shader='shaded', smooth=False)

    def set_data(self, x, y, z, width, heigth):
        x = np.array(x).flatten()
        y = np.array(y).flatten()
        z = np.array(z).flatten()

        pos = np.array(list(zip(x, y, z)))

        width = np.array(width).flatten()
        heigth = np.array(heigth).flatten()

        size = np.array(list(zip(width, width, heigth)))

        nCubes = np.prod(pos.shape[:-1])
        cubeVerts = np.mgrid[0:2, 0:2, 0:2].reshape(
            3, 8).transpose().reshape(1, 8, 3)
        cubeFaces = np.array([
            [0, 1, 2], [3, 2, 1],
            [4, 5, 6], [7, 6, 5],
            [0, 1, 4], [5, 4, 1],
            [2, 3, 6], [7, 6, 3],
            [0, 2, 4], [6, 4, 2],
            [1, 3, 5], [7, 5, 3]]).reshape(1, 12, 3)
        size = size.reshape((nCubes, 1, 3))
        pos = pos.reshape((nCubes, 1, 3))
        verts = cubeVerts * size + pos
        faces = cubeFaces + (np.arange(nCubes) * 8).reshape(nCubes, 1, 1)


        cm = pg.colormap.get('CET-R2')
        colors = cm.map(heigth/3.3, mode=pg.ColorMap.FLOAT).reshape((nCubes,1,4))
        vertex_colors = np.tile(colors, (1, 8, 1))
        md = gl.MeshData(vertexes=verts.reshape(nCubes*8, 3),
                         faces=faces.reshape(nCubes*12, 3),
                         vertexColors=vertex_colors.reshape(nCubes*8, 4))

        self.setMeshData(meshdata=md)


class BarPlot(gl.GLViewWidget):
    bar_height = np.zeros((4, 4))

    def __init__(self, *args):
        super().__init__(*args)
        self.opts['distance'] = 15
        self.setWindowTitle('pyqtgraph example: GLLinePlotItem')
        self.setGeometry(0, 0, 1920, 1080)
        self.show()

        # # create the background grids
        # gx = gl.GLGridItem()
        # gx.setSize(x=10, y=10, z=0)
        # gx.rotate(90, 0, 1, 0)
        # gx.translate(-5, 0, 0)
        # self.addItem(gx)
        # gy = gl.GLGridItem()
        # gy.setSize(x=10, y=10, z=0)
        # gy.rotate(90, 1, 0, 0)
        # gy.translate(0, -5, 0)
        # self.addItem(gy)
        # gz = gl.GLGridItem()
        # gz.setSize(x=10, y=10, z=0)
        # gz.translate(0, 0, -5)
        # self.addItem(gz)

        self.phase = 0
        x, y, z = np.meshgrid(np.arange(4), np.arange(4), [0])
        width = np.ones_like(x)*0.8
        height = np.cos(0.2*x + 0.2*y + self.phase)
        self.bar = MyBarItem(x, y, z, width, height)
        self.addItem(self.bar)



    def update_plot(self):
        x, y, z = np.meshgrid(np.arange(4), np.arange(4), [0])
        width = np.ones_like(x)*0.8
        height = np.cos(0.2*x + 0.2*y + self.phase)
        height = self.bar_height
        self.bar.set_data(x, y, z, width, height)
        self.phase -= 0.05

    def update_h(self, height):
        self.bar_height[:] = height[:]


# Start Qt event loop unless running in interactive mode.
if __name__ == '__main__':
    app = QtGui.QApplication(sys.argv)
    plot = BarPlot()
    timer = QtCore.QTimer()
    timer.timeout.connect(plot.update_plot)
    timer.start(10)
    app.exec()
