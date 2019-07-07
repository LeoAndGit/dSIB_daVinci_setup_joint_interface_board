
# -*- coding: utf-8 -*-
'''
Author: Leo
Copyright 2019, github/LeoAndGit>

'''

import sys
import time # 时间戳

from PyQt5 import QtCore, QtGui, QtWidgets
import pyqtgraph as pg
import qdarkstyle  # QSS样式表

from mainUI import Ui_Form #自动生成的界面
from deviceComScript import MySerial

import serial
import serial.tools.list_ports

class MyMainWindow(QtWidgets.QWidget):
    def __init__(self, parent=None):    
        super(MyMainWindow, self).__init__(parent)
        pg.setConfigOption('background', '#31363b')  # 设置背景为灰色
        pg.setConfigOption('foreground', 'w')  # 设置前景（包括坐标轴，线条，文本等等）为白色。
        #pg.setConfigOptions(antialias=True) # 使曲线看起来更光滑，而不是锯齿状

        self.ui = Ui_Form()
        self.ui.setupUi(self)
                
        ##创建线程实例
        self.thread1 = Thread1()

        self.thread1.sinOut1.connect(self.slotThread11)
        self.thread1.sinOut2.connect(self.slotThread12)

    def linkSlot(self):
        self.thread1.link()

    def startSlot(self):
        self.thread1.setAndStart(flag1 = "start")

    def stopSlot(self):
        self.thread1.setAndStart(flag1 = "stop")

    def buttonClicked(self):
    # click one of the nine buttons
        sender = self.sender().objectName()
        
        # 运动模式
        if sender == "pushButton_1": # ReleaseAll
            self.thread1.setAndStart(flag = "freeAll")
        
        elif sender == "pushButton_2": # LockAll
            self.thread1.setAndStart(flag = "lockAll")

        elif sender == "pushButton_3": # test at
            self.thread1.setAndStart(flag = "at")

        elif sender == "pushButton_4": # release J1
            self.thread1.setAndStart(flag = "release", value = b"1")

        elif sender == "pushButton_5": # release J2
            self.thread1.setAndStart(flag = "release", value = b"2")

        elif sender == "pushButton_6": # release J3
            self.thread1.setAndStart(flag = "release", value = b"3")

        elif sender == "pushButton_7": # release J4
            self.thread1.setAndStart(flag = "release", value = b"4")

        elif sender == "pushButton_8": # release J5
            self.thread1.setAndStart(flag = "release", value = b"5")

        elif sender == "pushButton_9": # release J6
            self.thread1.setAndStart(flag = "release", value = b"6")

    def slotThread11(self, text): # 结果输出
        # self.ui.textEdit.setPlainText(text)
        # 格式化时间
        timeText = time.strftime("%Y-%m-%d %H:%M:%S", time.localtime())
        self.ui.textEdit.append(timeText + " : " + text)  # 自动添加回车

    def slotThread12(self, text): # show value
        # deal with read value
        channel = text[-3:-2] # get channel number
        value = float(int(text[:6], 16)) # get HEX value and change it to int
        # change to a voltage value
        valueText = '{:.3f}'.format(value*2.5/16777215)
        if channel == "0":
            self.ui.lcdNumber_1.display(valueText)
        elif channel == "1":
            self.ui.lcdNumber_2.display(valueText)
        elif channel == "2":
            self.ui.lcdNumber_3.display(valueText)
        elif channel == "3":
            self.ui.lcdNumber_4.display(valueText)
        elif channel == "4":
            self.ui.lcdNumber_5.display(valueText)
        elif channel == "5":
            self.ui.lcdNumber_6.display(valueText)
        elif channel == "6":
            self.ui.lcdNumber_7.display(valueText)
        elif channel == "7":
            self.ui.lcdNumber_8.display(valueText)
        elif channel == "8":
            self.ui.lcdNumber_9.display(valueText)
        elif channel == "9":
            self.ui.lcdNumber_10.display(valueText)
        elif channel == "A":
            self.ui.lcdNumber_11.display(valueText)
        elif channel == "B":
            self.ui.lcdNumber_12.display(valueText)


class Thread1(QtCore.QThread):
    sinOut1 = QtCore.pyqtSignal(str)
    sinOut2 = QtCore.pyqtSignal(str)

    def _init_(self,parent=None):
        super(Thread1,self).__init__(parent)

    def setAndStart(self, flag = "", flag1 = "start", value = b""):
        self.flag = flag
        self.flag1 = flag1
        self.value = value
        # if value is not "stop":
         #    self.start()
        self.start()

    def link(self):
        """来自动连接在端口上的设备，会打开第一个串口
        注意：本方法可能无法跨平台使用，端口名相关部分未经确认
        一旦发现设备，将保持串口打开
        """
        self.serial = MySerial(
            port=None,
            baudrate=115200, # 串口波特率：115200bps
            # 莫名原因导致下述三项无法配置
            #bytesize=EIGHTBITS, # 8 位数据位
            #parity=PARITY_NONE, # 无校验位
            #stopbits=STOPBITS_ONE, # 1 位停止位
            timeout=0.2, # 读取超时0.2s
            xonxoff=False,
            rtscts=False,
            write_timeout=0.5, # 写超时0.5s
            dsrdtr=False,
            inter_byte_timeout=None,
            exclusive=None)
        if self.serial.is_open is True: #检测是否已经开启
            self.serial.close()
        # 获取串口列表
        self.plist = list(serial.tools.list_ports.comports())

        if len(self.plist) <= 0: # 无串口
            self.sinOut1.emit("no port")
        else:
            # 连接第一个串口
            plist_0 = list(self.plist[0])
            serialName = plist_0[0]
            self.serial.port = serialName
            self.serial.open()
            self.sinOut1.emit(serialName)
            self.serial.reset_input_buffer()

    def run(self):
        try:
            while self.flag1 is "start":
                # keep update data
                self.serial.write(b"AT+READALL\r\n") # ask for date update
                for num in range(13):
                    answer = self.serial.read_until()
                    if num == 12: # final read
                        if answer[-4:] == b"OK\r\n":
                            # self.sinOut1.emit("Success")
                            pass
                        else:
                            self.serial.reset_input_buffer() # 清空输入缓存
                            self.sinOut1.emit("Error")
                            self.flag1 = "stop" # stop update
                    else: # update value
                        self.sinOut2.emit(str(answer, encoding="utf-8"))

                if self.flag is "release":
                    self.serial.write(b"AT+FREE=" +  self.value + b"\r\n")
                    answer = self.serial.read_until()
                    # 后四个
                    if answer[-4:] == b"OK\r\n":
                        self.sinOut1.emit("Success")
                    else:
                        self.serial.reset_input_buffer() # 清空输入缓存
                        self.sinOut1.emit("Error")
                    self.flag = "" # clear

                if self.flag is "freeAll":
                    self.serial.write(b"AT+FREEALL\r\n")
                    answer = self.serial.read_until()
                    # 后四个
                    if answer[-4:] == b"OK\r\n":
                        self.sinOut1.emit("Success")
                    else:
                        self.serial.reset_input_buffer() # 清空输入缓存
                        self.sinOut1.emit("Error")
                    self.flag = "" # clear

                if self.flag is "lockAll":
                    self.serial.write(b"AT+LOCKALL\r\n")
                    answer = self.serial.read_until()
                    # 后四个
                    if answer[-4:] == b"OK\r\n":
                        self.sinOut1.emit("Success")
                    else:
                        self.serial.reset_input_buffer() # 清空输入缓存
                        self.sinOut1.emit("Error")
                    self.flag = "" # clear

                if self.flag is "at":
                    self.serial.write(b"AT\r\n")
                    answer = self.serial.read_until()
                    # 后四个
                    if answer[-4:] == b"OK\r\n":
                        self.sinOut1.emit("Success")
                    else:
                        self.serial.reset_input_buffer() # 清空输入缓存
                        self.sinOut1.emit("Error")
                    self.flag = "" # clear

                self.msleep(100) # 刷新率接近10Hz

        except serial.SerialException as e: # 串口操作出错
            self.sinOut1.emit(str(e))
           


if __name__=="__main__":  
    app = QtWidgets.QApplication(sys.argv)
    # 设置样式表
    app.setStyleSheet(qdarkstyle.load_stylesheet_pyqt5())
    myWin = MyMainWindow()
    myWin.show()#向框架添加绘制事件
    sys.exit(app.exec_())