#!/usr/bin/env python

'''
         MEGN540 Mechatronics Lab
    Copyright (C) Andrew Petruska, 2021.
       apetruska [at] mines [dot] edu
          www.mechanical.mines.edu
'''

'''
    Copyright (c) 2021 Andrew Petruska at Colorado School of Mines

    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"), to deal
    in the Software without restriction, including without limitation the rights
    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
    copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in all
    copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
    SOFTWARE.

'''
# GUI IMPORTS
import tkinter
from tkinter import filedialog
from tkinter import *

# FOR THREADING AND MUTEX PROTECTION (used in serial interface primarily) 
from threading import Thread, Lock
import collections  # FOR DEQUEUE USED IN DATA STORAGE AND CALLBACK QUEUES

# FOR SERIAL COMMUNICATIONS
import serial # FOR SERIAL INTERFACE
import struct # FOR BINARY DATA INTERFACING
import time   # FOR TIME STAMPING DATA



# FOR REALTIME PLOT
import matplotlib.pyplot as plt 
import matplotlib.animation as animation
from matplotlib.backends.backend_tkagg import(FigureCanvasTkAgg,NavigationToolbar2Tk)


class SerialData:
    def __init__(self):

        self.isRun = False
        # self.isReceiving = False
        self.thread = None
        self.callbackfunction = collections.deque()
        self.callback_list_mutex = Lock()
        self.serial_read_write_mutex = Lock()
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
        try:
            value = struct.unpack(self.dataFormat, self.rawData)
        except:
            return
        
        data = [];
        
        '''for i in range(len(value)):         
            if self.dataFormat[i+1-rep_ind] == 'c':
                data.append(value[i].decode('ascii'))
            elif self.dataFormat[i+1-rep_ind] == 'b' or self.dataFormat[i] == 'h':
                data.append(int(value[i]))
            else:
                data.append(value[i])'''
        rep = 1
        ind = 0
        for fmt in self.dataFormat:
            if fmt is '<':
                continue
            
            if rep == 1:
                try:
                    rep = int(str(fmt))
                    continue
                except:
                    rep = 1

            for i in range(rep):
                if fmt == 'c':
                    if rep == 1 or i == 0:
                        data.append(value[ind].decode('ascii'))
                    else:
                        data[-1] += value[ind].decode('ascii')
                        
                elif fmt == 's':
                    data.append(value[ind].decode('ascii'))
                    ind += 1
                    break
                elif fmt == 'b' or fmt == 'h':
                    data.append(int(value[ind]))
                else:
                    data.append(value[ind])
                    
                ind += 1
            rep = 1

                
        self.callback_list_mutex.acquire()
        try:
            for function in self.callbackfunction:
                function(data)
        finally:
            self.callback_list_mutex.release()

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
        
        self.rawData = bytearray(0)
        
        while self.isRun:
            try:
                if self.defined_data_mode and self.serialConnection.in_waiting >= self.dataNumBytes and self.dataNumBytes > 0:
                    self.rawData = bytearray(self.dataNumBytes)
                    self.serial_read_write_mutex.acquire()
                    try:
                        self.serialConnection.readinto(self.rawData)
                    finally:
                        self.serial_read_write_mutex.release()
                    self.parseData()
                
                elif (not self.defined_data_mode) and self.serialConnection.in_waiting and  self.dataNumBytes == -1:
                    self.dataNumBytes = struct.unpack('b',self.serialConnection.read(1))[0]
                
                elif (not self.defined_data_mode) and self.serialConnection.in_waiting >= self.dataNumBytes and self.dataNumBytes > 0 :
                    self.serial_read_write_mutex.acquire()
                    try:
                        tmp = self.serialConnection.read(1)
                    finally:
                        self.serial_read_write_mutex.release()
                    
                    self.dataNumBytes -= 1
                    tmp_uchar = struct.unpack('b',tmp)[0]
                    
                    if tmp_uchar is not 0:
                        self.dataFormat = self.dataFormat + struct.unpack('c',tmp)[0].decode('ascii')
                        try:
                            try:
                                i = int(self.dataFormat[-1])
                            except:
                                i = None
                                
                            if i is None:
                                struct.calcsize(self.dataFormat) # check if its a valid format skip ones that end in a number
                        except:
                            print("num bytes: " + str(self.dataNumBytes) + " attempt fmt: " + self.dataFormat)
                            self.dataNumBytes = -1
                            self.dataFormat = "<"
                    else:
                        if struct.calcsize(self.dataFormat) == self.dataNumBytes:
                            # all is as expected
                            self.rawData = bytearray(self.dataNumBytes)
                            self.serial_read_write_mutex.acquire()
                            try:
                                self.serialConnection.readinto(self.rawData)
                            finally:
                                self.serial_read_write_mutex.release()
                            self.parseData()
                        self.dataNumBytes = -1
                        self.dataFormat = "<"
                
                elif self.dataNumBytes == 0:
                    self.dataNumBytes = -1
                
                else:
                    time.sleep(0.001) # recheck serial every 5ms
                    
                        
            
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


        try:
            if len(data) == 1:
                msg = struct.pack("<"+data_format_str,data[0])
            elif len(data) == 2:
                msg = struct.pack("<"+data_format_str,data[0],data[1])
            elif len(data) == 3:
                msg = struct.pack("<"+data_format_str,data[0],data[1],data[2])
            elif len(data) == 4:
                msg = struct.pack("<"+data_format_str,data[0],data[1],data[2],data[3])
            else:
                return (False, "Data Length Unsupported")
        except:
            return (False, "Format/Entry Mismatch" )
            
        if self.isConnected():    
            if self.serialConnection:
                self.serial_read_write_mutex.acquire()
                try:
                    self.serialConnection.write(msg)
                finally:
                    self.serial_read_write_mutex.release()
                return True, None
            else:
                return (False, 'Port Not Writeable')
        else:
            return (False, 'Not Connected')

    def close(self, on_shutdown=False):
        if self.isConnected():
            self.isRun = False
            self.thread.join()
            self.thread = None
            self.serialConnection.close()
            if not on_shutdown:
                print('Serial Port ' + self.port + ' Disconnected.\n')

    def registerCallback(self, function):
        self.callback_list_mutex.acquire()
        try:
            self.callbackfunction.append(function)
        finally:
            self.callback_list_mutex.release()
            

    def removeCallback(self, function):
        self.callback_list_mutex.acquire()
        try:
            self.callbackfunction.remove(function)
        finally:
            self.callback_list_mutex.release()
            


class RecordData:
    def __init__(self):
        self.csvData = collections.deque(maxlen=100000)
        self.csvTime = collections.deque(maxlen=100000)
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
            filename = filedialog.asksaveasfilename(title="test", filetypes=(("csv files", "*.csv"), ("all files", "*.*")))
            if filename:
                file = open(filename,'w');
                time_ind = 0;
                for val in self.csvData:
                    file.write(str(self.csvTime[time_ind]))
                    time_ind += 1
                    for e in val:
                        file.write(", " + str(e) )
                    
                    file.write('\n')
                
                file.close()


class RealTimePlot():
    def __init__(self, plotLength=500, refreshTime=10):
               
        self.gui_main = None       
        self.window = None
        self.plotMaxLength = plotLength
        
        self.data  = collections.deque( maxlen=plotLength)
        self.times = collections.deque( maxlen=plotLength)
        self.plotTimer = 0
        self.previousTimer = 0
        self.valueLast = None
        self.p = None
        self.fig = None
        
        self.t_start = time.perf_counter()
        self.values_queue = collections.deque(maxlen=plotLength)
        self.times_queue  = collections.deque(maxlen=plotLength)
        
        self.plotTimer = 0
        self.previousTimer = 0
        self.timeText = None
        
        self.input_index = 0;
        
        self.pltInterval = refreshTime  # Refresh period [ms]
        
        self.data_mutex = Lock()

    def updatePlotData(self,args=None): #, frame, lines, lineValueText, lineLabel, timeText):
#        while self.isRunning:
        
        if len(self.times_queue):
            currentTimer = time.perf_counter()
            self.plotTimer = int((currentTimer - self.previousTimer) * 1000)
            if self.plotTimer > 1:
                self.previousTimer = currentTimer
                self.timeText.set_text('Plot Interval = ' + str(self.plotTimer) + 'ms')
        else:
            return
       
        valueLast = []

        self.data_mutex.acquire()

        while len(self.times_queue):
            try:
                valueLast = self.values_queue[-1][self.input_index]
                time_val = self.times_queue[-1]
                valueLast = float(valueLast) # make sure its a number
                self.data.append(valueLast)  # latest data point and append it to array
                self.times.append(time_val)
                self.values_queue.clear()
                self.times_queue.clear()
            except:
                break
        

        self.data_mutex.release()
        
        self.lines.set_data(self.times, self.data)
        if len(self.data):
            self.lineValueText.set_text('[' + self.lineLabel + " IND: " +str(self.input_index) + '] = ' + str(round(self.data[-1],3)))
        
        if len(self.times) > 5:
            #self.fig.canvas.restore_region(self.background)
            self.ax.set_xlim(self.times[0],self.times[-1])
            
            min_ylim = min(self.data)
            max_ylim = max(self.data)
            
            if min_ylim == max_ylim:
                if min_ylim == 0:
                    min_ylim = -1
                    max_ylim = 1
                else:
                    min_ylim = min_ylim*.2
                    max_ylim = max_ylim*1.2
            
            
            self.ax.set_ylim(min_ylim - (max_ylim-min_ylim)/10, max_ylim + (max_ylim-min_ylim)/10)

    def addValue(self, value):
        self.data_mutex.acquire()
        self.values_queue.append(value)
        self.times_queue.append(time.perf_counter()-self.t_start)
        self.data_mutex.release()
        
    def changePlotIndex(self, index):
        self.input_index = index
        self.times.clear()
        self.data.clear()

    def setupPlot(self):  # retrieve data
        xmin = 0
        xmax = self.plotMaxLength
        ymin = -1
        ymax = 1050
        self.fig = plt.figure()
        self.ax = plt.axes( autoscale_on=True)#xlim=(xmin, xmax), ylim=(float(ymin - (ymax - ymin) / 10), float(ymax + (ymax - ymin) / 10)))
        self.ax.set_title('Arduino Analog Read')
        self.ax.set_xlabel("time")
        self.ax.set_ylabel("AnalogRead Value")
        
        self.canvas = FigureCanvasTkAgg(self.fig, master=self.window)
        self.canvas.draw()
        self.canvas.get_tk_widget().pack(side=tkinter.TOP, fill=tkinter.BOTH, expand=1)
        
        toolbar = NavigationToolbar2Tk(self.canvas,self.window)
        toolbar.update()
        self.canvas.get_tk_widget().pack(side=tkinter.TOP, fill=tkinter.BOTH, expand=1)

        self.lineLabel = 'Sensor Value'
        self.timeText = self.ax.text(0.50, 0.95, '', transform=self.ax.transAxes)
        self.lines = self.ax.plot([], [], label=self.lineLabel)[0]
        self.lineValueText = self.ax.text(0.50, 0.90, '', transform=self.ax.transAxes)
  
        # START THE PLOT ANIMATION
        self.anim = animation.FuncAnimation(self.fig, self.updatePlotData, interval=self.pltInterval)
        
    
    def isOk(self):
        return self.anim is not None

    def close(self):
        if self.anim is not None:
            self.anim.event_source.stop()
        self.anim = None
 
        self.window.withdraw()
        
        '''if self.window is not None:
            self.window.quit() # stops main loop
            self.window.destroy() # Destroys window and all child widgets
        '''

    def Start(self, main=None):
        if self.window is None:
            self.window = Toplevel(main)
            self.window.title("Real Time Plot")
            self.window.geometry("800x600")
            self.window.protocol("WM_DELETE_WINDOW", self.close)
            self.gui_main = main
        self.setupPlot()

