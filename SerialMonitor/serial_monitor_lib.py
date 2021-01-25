#!/usr/bin/env python

from tkinter import filedialog
from tkinter import *
import multiprocessing
import queue
from threading import Thread
import serial
import time
import collections
import matplotlib.pyplot as plt
import matplotlib.animation as animation
import struct
import csv
import pandas as pd
from array import *


class SerialData:
    def __init__(self):
        self.dataNumBytes = None
        self.rawData = None
        self.isRun = True
        # self.isReceiving = False
        self.thread = None
        self.callbackfunction = collections.deque()
        self.port = None
        self.baud = None
        self.serialConnection = None
        self.setDataFormat('c')

    def openPort (self, serialPort='COM5', serialBaud=9600):
        self.port = serialPort
        self.baud = serialBaud

        print('Trying to connect to: ' + str(serialPort) + ' at ' + str(serialBaud) + ' BAUD.')
        try:
            self.serialConnection = serial.Serial(serialPort, serialBaud)
            if(self.serialConnection.isOpen() == False):
                self.serialConnection.open()
                
            print('Connected to ' + str(serialPort) + ' at ' + str(serialBaud) + ' BAUD.')
            self.readSerialStart()
        except (RuntimeError, TypeError, NameError):
            print("Failed to connect with " + str(serialPort) + ' at ' + str(serialBaud) + ' BAUD.')
            print(NameError)

    def isConnected(self):
        return self.thread is not None

    def readSerialStart(self):
        if self.thread is None:
            self.thread = Thread(target=self.backgroundThread)
            self.thread.start()
            '''while self.isReceiving is not True:
                time.sleep(0.1)'''

    def parseData(self):
        value = struct.unpack("<"+self.dataFormat, self.rawData)
        for function in self.callbackfunction:
            function(value)

    def setDataFormat(self, new_format):
        self.dataFormat = new_format
        self.dataNumBytes = struct.calcsize("<"+new_format)
        self.rawData = bytearray(self.dataNumBytes)


    def backgroundThread(self):  # retrieve data
        self.serialConnection.reset_input_buffer()
        print('starting background thread...\n')
        while self.isRun:
            if self.serialConnection.in_waiting >=  self.dataNumBytes:
                self.serialConnection.readinto(self.rawData)
                self.parseData()
            else:
                time.sleep(0.005) # recheck serial every 5ms

    def write(self, cmd, float1, float2):
        if len(cmd) > 1:
            print("Can only send a one character command identifier")
            return False
        
        if cmd and float1 and float2:
            msg = struct.pack("<c2f",cmd.encode(),float1,float2)
        elif cmd and float1:
            msg = struct.pack("<cf",cmd.encode(),float1)
        elif cmd:
            msg = struct.pack("<c",cmd.encode())
        else:
            print("Invalid Write String Combination")
            return False
        
        if self.serialConnection: # and self.serialConnection.writable is True:
            self.serialConnection.write(msg)
            return True
        else:
            print('Error port not writeable!')
            return False

    def close(self):
        if self.isConnected():
            self.isRun = False
            self.thread.join()
            self.serialConnection.close()
            print('Searial Port Disconnected.\n')

    def registerCallback(self, function):
        self.callbackfunction.append(function)

    def removeCallback(self, function):
        self.callbackfunction.remove(function)


class RecordData:
    def __init__(self):
        self.csvData = []
        self.csvTime = []
        self.is_recording = False

    def startRecording(self):
        self.is_recording = True
        print("start recording")

    def addData(self, value):
        if self.is_recording is True:
            currentTimer = time.perf_counter()
            self.csvData.append(value)
            self.csvTime.append(currentTimer)

    def stopRecording(self):
        if self.is_recording:
            self.is_recording = False
            print("Stop recording")
    
    def isRecording(self):
        return self.is_recording

    def saveData(self):
        if self.csvData:
            d = {'TIME (ms)': self.csvTime, 'VALUE': self.csvData}
            df = pd.DataFrame(d)
            filename = filedialog.asksaveasfilename(title="test", filetypes=(("csv files", "*.csv"), ("all files", "*.*")))
            if filename:
                df.to_csv(filename)
                self.csvData = []
                self.csvTime = []


class RealTimePlot:
    def __init__(self, plotLength=100, refreshTime=50):
        self.plotMaxLength = plotLength
        self.data = collections.deque([0] * plotLength, maxlen=plotLength)
        self.plotTimer = 0
        self.previousTimer = 0
        self.valueLast = None
        self.p = None
        self.fig = None
        self.q = None
        self.pltInterval = refreshTime  # Refresh period [ms]

    def updatePlotData(self, frame, lines, lineValueText, lineLabel, timeText):
        currentTimer = time.perf_counter()
        self.plotTimer = int((currentTimer - self.previousTimer) * 1000)
        self.previousTimer = currentTimer
        timeText.set_text('Plot Interval = ' + str(self.plotTimer) + 'ms')
        valueLast = self.q.get()

        self.data.append(valueLast)  # latest data point and append it to array
        lines.set_data(range(self.plotMaxLength), self.data)
        lineValueText.set_text('[' + lineLabel + '] = ' + str(self.valueLast))

    def addValue(self, value):
        if self.q != None:
            try:
                self.q.get_nowait()
            except queue.Empty:
                pass

            self.q.put(value)

    def backgroundThread(self):  # retrieve data
        xmin = 0
        xmax = self.plotMaxLength
        ymin = -1
        ymax = 1050
        self.fig = plt.figure()
        ax = plt.axes(xlim=(xmin, xmax), ylim=(float(ymin - (ymax - ymin) / 10), float(ymax + (ymax - ymin) / 10)))
        ax.set_title('Arduino Analog Read')
        ax.set_xlabel("time")
        ax.set_ylabel("AnalogRead Value")

        lineLabel = 'Sensor Value'
        timeText = ax.text(0.50, 0.95, '', transform=ax.transAxes)
        lines = ax.plot([], [], label=lineLabel)[0]
        lineValueText = ax.text(0.50, 0.90, '', transform=ax.transAxes)
        anim = animation.FuncAnimation(self.fig, self.updatePlotData, fargs=(lines, lineValueText, lineLabel, timeText),
                                       interval=self.pltInterval)

        plt.legend(loc="upper left")
        plt.show()
        # self.fig = None

    def close(self):
        # if self.fig != None:
        plt.close(self.fig)
        if self.p is not None:
            self.q.close()
            self.q = None
            self.p.terminate()
        self.p = None

    def Start(self):
        self.q = multiprocessing.Queue(maxsize=2)
        self.p = multiprocessing.Process(target=self.backgroundThread)
        self.p.start()


#def printMe(value):
#    print("value is: " + str(value))
#
#
#def main():
#    portName = 'COM5'
#    baudRate = 9600
#
#    # 4 bytes in 1 data point
#    s = SerialData()
#    s.openPort(portName, baudRate)
#    # s.registerCallback(printMe)
#
#    p = RealTimePlot()
#    s.registerCallback(p.addValue)
#    p.Start()
#
#    r = RecordData()
#    s.registerCallback(r.addData)
#    r.startRecording()
#    time.sleep(10)
#    r.stopRecording()
#
#    s.close()
#
#    r.saveData()
#
#    p.close()
#
#
#if __name__ == '__main__':
#    main()
