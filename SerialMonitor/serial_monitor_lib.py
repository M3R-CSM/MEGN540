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

        self.isRun = False
        # self.isReceiving = False
        self.thread = None
        self.callbackfunction = collections.deque()
        self.port = None
        self.baud = None
        self.serialConnection = None
        
        self.defined_data_mode = True
        self.dataNumBytes = -1
        self.dataFormat = "<"
        self.rawData = None

    def openPort (self, serialPort='COM5', serialBaud=9600):
        
        if self.isRun:
            close()
        
        self.port = serialPort
        self.baud = serialBaud

        print('Trying to connect to: ' + str(serialPort) + ' at ' + str(serialBaud) + ' BAUD.')
        try:
            self.serialConnection = serial.Serial(serialPort, serialBaud)
            if(self.serialConnection.isOpen() == False):
                self.serialConnection.open()
                
            print('Connected to ' + str(serialPort) + ' at ' + str(serialBaud) + ' BAUD.')
            self.readSerialStart()
        except:
            print("Failed to connect with " + str(serialPort) + ' at ' + str(serialBaud) + ' BAUD.')

    def isConnected(self):
        return self.isRun 

    def readSerialStart(self):
        if not self.isRun:
            self.thread = Thread(target=self.backgroundThread)
            self.isRun = True
            self.thread.start()

    def parseData(self):
        value = struct.unpack(self.dataFormat, self.rawData)
        data = [];
        
        for i in range(len(value)):
            if self.dataFormat[i+1] == 'c':
                data.append(value[i].decode('ascii'))
            elif self.dataFormat[i+1] == 'b' or self.dataFormat[i] == 'h':
                data.append(int(value[i]))
            else:
                data.append(value[i])
                     
        for function in self.callbackfunction:
            function(data)

    def setDataFormat(self, new_format):
        if new_format != "Dynamic":
            try:
                self.defined_data_mode = True
                self.dataFormat = "<"+new_format
                self.dataNumBytes = struct.calcsize(self.dataFormat)
                self.rawData = bytearray(self.dataNumBytes)
            except:
                print("Invalid Format: " + new_format)
                return False
        else:
            self.defined_data_mode = False
            self.dataFormat = "<"
            self.dataNumBytes = -1
            self.rawData = None
        
        return True

    def backgroundThread(self):  # retrieve data
        self.serialConnection.reset_input_buffer()
        print('Serial Monitoring Thread Started\n')
        
        while self.isRun:
            try:
                if self.defined_data_mode and self.serialConnection.in_waiting >= self.dataNumBytes:
                    self.rawData = bytearray(self.dataNumBytes)
                    self.serialConnection.readinto(self.rawData)
                    self.parseData()
                
                elif (not self.defined_data_mode) and self.serialConnection.in_waiting and  self.dataNumBytes == -1:
                    self.dataNumBytes = struct.unpack('b',self.serialConnection.read(1))[0]
                
                elif (not self.defined_data_mode) and self.serialConnection.in_waiting >= self.dataNumBytes and self.dataNumBytes > 0 :
                    tmp = self.serialConnection.read(1)
                    self.dataNumBytes -= 1
                    tmp_uchar = struct.unpack('b',tmp)[0]
                    
                    if tmp_uchar != 0:
                        self.dataFormat = self.dataFormat + struct.unpack('c',tmp)[0].decode('ascii')
                    else:                       
                        self.rawData = bytearray(self.dataNumBytes)
                        self.serialConnection.readinto(self.rawData)
                        self.parseData()
                        self.dataNumBytes = -1
                        self.dataFormat = "<"
                
                elif self.dataNumBytes == 0:
                    self.dataNumBytes = -1
                
                else:
                    time.sleep(0.005) # recheck serial every 5ms
            
            except:
                self.isRun = False
                self.thread = None
                self.serialConnection.close()
                print('Connection Lost\n')
                
    def write(self, data, data_format):
        try:
            index = 0
            for d in data:
                if data_format[index] == 'c':
                    data[index] = d.encode()
                elif data_format[index] == 'f':
                    data[index] = float(d)
                else:
                    data[index] = int(d)
                index += 1
                
        except:
            return (False, 'Format/Entry Mismatch')
        
        data_format_str = ""
        for e in data_format:
            data_format_str += e
        print(data_format_str)
        print(data)
        try:
            if len(data) == 1:
                msg = struct.pack("<"+data_format_str,data[0])
            elif len(data) == 3:
                msg = struct.pack("<"+data_format_str,data[0],data[1],data[2])
            elif len(data) == 2:
                msg = struct.pack("<"+data_format_str,data[0],data[1])
            else:
                return (False, "Data Length Unsupported")
        except:
            return (False, "Format/Entry Mismatch" )
            
        if self.serialConnection: # and self.serialConnection.writable is True:
            self.serialConnection.write(msg)
            return True, None
        else:
            return (False, 'Port Not Writeable!')

    def close(self):
        if self.isConnected():
            self.isRun = False
            self.thread.join()
            self.thread = None
            self.serialConnection.close()
            print('Searial Port ' + self.port + ' Disconnected.\n')

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
