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

import itertools # interating through multiple lists

# GUI IMPORTS
from tkinter import *      # for gui general
from tkinter.ttk import *  # for combobox
from threading import Lock

# BACKGROUND MEGN540 FUNCTIONS
import serial_monitor_lib

# IMPORT FOR DEQUEUE
import collections

# FOR RUNNIGN SET OF CSV COMMANDS
import csv
from tkinter import filedialog
from tkinter import simpledialog
import struct


class GuiSetup:
    def __init__(self):
        self.gui = Tk()
        self.gui.title("MEGN540 Serial Monitor & Plotter")
        
        # Contact Info
        self.contact = Label(text="apetruska@mines.com").place(x=250, y=437)
        
        self.ok = True # flag to indicate shutdown has initiated
        self.gui.protocol("WM_DELETE_WINDOW", self.close_window)

        # Setup Serial Object
        self.serial_object = serial_monitor_lib.SerialData()
        self.serial_object.registerCallback(self.addValue)
        
        self.serial_data_lock = Lock()
        self.serial_data = collections.deque()

        # frames
        frame_1 = Frame(self.gui, height=300, width=480, relief='groove')
        frame_1.pack(side=TOP)
        frame_1t = Frame(frame_1, height = 275, width = 460, relief='groove')
        frame_1t.pack(side=TOP)
       
        frame_2 = Frame(self.gui, height=200, width=480, relief='groove')
        frame_2.pack(side=BOTTOM, fill=Y)
        
        # Text window and scroll bar
        self.text_box_update_Hz = 10  # 10 hz update
        self.scrollbar = Scrollbar(frame_1t)
        self.scrollbar.pack(side = RIGHT, fill = Y)
        self.text = Text(frame_1t, height=12, yscrollcommand=self.scrollbar.set)
        self.text.pack(side=LEFT, fill=X)
        self.scrollbar.config(command=self.text.yview)
        
        # Filter Response box
        self.data_entry_filt = Entry(self.gui)
        self.data_entry_filt.place(x=5, y=210, width=70)
        Label(self.gui, text="Display Filter").place(x=76, y=213)


        # Send Button & Input
        xLoc = 5
        yLoc = 250
        self.button1 = Button(text="Send", command=self.send, width=6).place(x=xLoc, y=yLoc)
        xLoc += 75
        yLoc += 5
        
        self.out_selection = ["c", "b", "B", "h", "f"]
        self.combobox_out = []
        self.data_entry = []
        self.combobox_out.append(Combobox(self.gui, values = self.out_selection, width=3 ))
        self.combobox_out[-1].place(x=xLoc+15, y=yLoc-20)
        self.combobox_out[-1].current(0)
        self.data_entry.append(Entry(self.gui))
        self.data_entry[-1].place(x=xLoc, y=yLoc, width=70)
        xLoc += 75
        
        self.combobox_out.append(Combobox(self.gui, values = self.out_selection, width=3 ))
        self.combobox_out[-1].place(x=xLoc+15, y=yLoc-20)
        self.combobox_out[-1].current(4)
        self.data_entry.append( Entry(self.gui))
        self.data_entry[-1].place(x=xLoc, y=yLoc, width=70)
        xLoc += 75
        
        self.combobox_out.append(Combobox(self.gui, values = self.out_selection, width=3 ))
        self.combobox_out[-1].place(x=xLoc+15, y=yLoc-20)
        self.combobox_out[-1].current(4)
        self.data_entry.append(Entry(self.gui))
        self.data_entry[-1].place(x=xLoc, y=yLoc, width=70)
        
        xLoc += 75
        self.combobox_out.append(Combobox(self.gui, values = self.out_selection, width=3 ))
        self.combobox_out[-1].place(x=xLoc+15, y=yLoc-20)
        self.combobox_out[-1].current(4)
        self.data_entry.append(Entry(self.gui))
        self.data_entry[-1].place(x=xLoc, y=yLoc, width=70)


        # Incomming Message Formatting String
        xLoc += 100
        yLoc -= 20
        self.charnum = Label(text="Incoming Format").place(x=xLoc-20, y=yLoc-20)
        self.input_format_sv = StringVar()
        self.input_format_sv.trace("w", lambda name, index, mode, sv=self.input_format_sv: self.change_input_format_callback(sv))
        self.format_entry = Entry(width=7, textvariable=self.input_format_sv)
        self.format_entry.place(x=xLoc, y=yLoc)
        self.input_format_sv.set("c")
        
        self.combobox = Combobox(self.gui, values = ["Hex", "int8_t", "uint8_t", "Char", "Fixed", "Dynamic"], width=7 )
        self.combobox.bind("<<ComboboxSelected>>", self.cb_selection_changed)
        self.combobox.place(x=xLoc, y=yLoc+20)
        self.combobox.current(0)
        self.cb_selection_changed(None)
        
        
        
        # Serial baudrate box
        xLoc = 130
        yLoc = 400
        self.baud = Label(text="Baud").place(x=xLoc, y=yLoc)
        self.baud_entry = Entry(width=7)
        self.baud_entry.place(x=(xLoc+50), y=yLoc)
        self.baud_entry.insert(1,256000)

        # Serial Port Input Box
        yLoc +=35
        self.port = Label(text="Port").place(x=xLoc, y=yLoc)
        self.port_entry = Entry(width=25)
        self.port_entry.place(x=(xLoc+50), y=yLoc)
        self.port_entry.insert(1,"/dev/ttyZumoCar")

        # button
        yLoc += - 40
        self.button_connect = Button(   text=" Connect  ", command=self.connectToSerial)
        self.button_connect.place(x=15, y=yLoc)

        yLoc += - 55
        xLoc = 15
        self.recording = Button(text="Send CSV Commands", command=self.loadCSV_Commands)
        self.recording.place(x=xLoc, y=yLoc)
        
        xLoc += 170
        self.recording = Button(text="Start Recording", command=self.dataRecordingStartStop)
        self.recording.place(x=xLoc, y=yLoc)
        
        xLoc += 120
        self.graphing = Button(text="Open Plot", command=self.plotWindowOpenClose)
        self.graphing.place(x=xLoc, y=yLoc)
             
        xLoc += 95
        yLoc += 3
        self.plot_select_values = [0]
        self.plot_select = Combobox(self.gui, values = self.plot_select_values, width=3 )
        self.plot_select.bind("<<ComboboxSelected>>", self.plot_selection_changed)
        self.plot_select.place(x=xLoc, y=yLoc)
        self.plot_select.current(0)
        self.plot_select.config(state='disabled')


        
        self.plotObject = None
        self.recordObject = None
        
        self.update_job = None
        self.auto_send_job = None
        
        self.update_gui()
        
    def plot_selection_changed(self,unused=None):
        selection = self.plot_select.current()
        if self.plotObject is not None:
            self.plotObject.changePlotIndex(selection)
            
            
    def cb_selection_changed(self,unused):
        selection = self.combobox.current()
      
        if selection is 0: # HEX
            self.input_format_sv.set('B')
            self.format_entry.config(state='disabled')
        
        elif selection is 1: #int8_t
            self.input_format_sv.set('b')
            self.format_entry.config(state='disabled')
            
        elif selection is 2: #uint8_t
            self.input_format_sv.set('B')
            self.format_entry.config(state='disabled')

        elif selection is 3: #Char
            self.input_format_sv.set('c')
            self.format_entry.config(state='disabled')
            
        elif selection is 4: #Fixed
            self.input_format_sv.set('c')
            self.format_entry.config(state='enabled')
            
        else: #Dynamic
            self.input_format_sv.set("Dynamic")
            self.format_entry.config(state='disabled')
   
    def change_input_format_callback(self, sv):
        ok_format = self.serial_object.setDataFormat(sv.get())
        if not ok_format:
            self.format_entry.delete(self.format_entry.index(INSERT)-1)

    def addValue(self, value):
        self.serial_data_lock.acquire()
        self.serial_data.append(value)
        self.serial_data_lock.release()

    def connectToSerial(self):
        """The function initiates the Connection to the UART device with the Port and Buad fed through the Entry
        boxes in the application.

        The radio button selects the platform, as the serial object has different key phrases
        for Linux and Windows. Some Exceptions have been made to prevent the app from crashing,
        such as blank entry fields and value errors, this is due to the state-less-ness of the
        UART device, the device sends data at regular intervals irrespective of the master's state.
        The other Parts are self explanatory.
        """
        '''baud = serial_monitor_lib.SerialData(baud)
        port = serial_monitor_lib.SerialData(port)'''
        if not self.serial_object.isConnected():
            # global serial_object
            port = self.port_entry.get()
            baud = self.baud_entry.get()

            try:
                self.serial_object.openPort( str(port), baud)
            except ValueError:
                print("Can't Open Specified Port")
                
        else:  # Disconnect
            try:
                self.serial_object.close()
            except AttributeError:
                print("Closed without using it -_-")
                
        if self.serial_object.isConnected():
            self.button_connect.configure(text="Disconnect")
        else:
            self.button_connect.configure(text=" Connect ")


    def debug_check_gui(self):
        counter = 0
        while self.ok:
            self.addValue("Line " + str(counter))
            counter += 1
            time.sleep(.3)

    def update_gui(self):
        """" This function updates the text box with incomming serial data. """        
  #      while self.ok: # self.ok gets set to False on window exit
            
        # Update Button States
        try:
            if self.serial_object.isConnected():
                self.button_connect.configure(text="Disconnect")
            else:
                self.button_connect.configure(text=" Connect ")
        except:
            pass
       
        self.serial_data_lock.acquire()
        while len(self.serial_data):
            # Insert new serial data if any
            try:
                dat = self.serial_data.popleft()            
                
                if len(dat) is not len(self.plot_select_values):
                    self.plot_select_values.clear()
                    for i in range(len(dat)):
                        self.plot_select_values.append(i)
                    self.plot_select.current(0)
                    self.plot_select.config(values=self.plot_select_values)
                    
                #print( "dat 0: " + str(dat[0]) + " list: " + str(self.data_entry_filt.get()))
                skip = False
                for v in self.data_entry_filt.get():
                    if v == dat[0] :
                        skip = True
                        break;
                    
                if not skip:
                    if self.combobox.current():
                        self.text.insert(END, str(dat))  # puts text data on monitor
                    else:
                        for v in dat:
                            self.text.insert(END, hex(v))  # puts text data on monitor
                        
                    self.text.insert(END, "\n")
                    if self.text.yview()[1] > .9: # make slider stickey if at bottom
                        self.text.yview(END)
            except:
                 break
        self.serial_data_lock.release()

            
        if (self.plotObject is not None) and not self.plotObject.isOk():
            self.plotWindowOpenClose() #will disconnect the grap and call close and change buttons etc
            
        self.update_job = self.gui.after(int(1000/self.text_box_update_Hz),self.update_gui)
        
            
    def close_window(self):
        """ This function is for some internal cleanup operations on closeing to make sure
        we leave the serialport in a good state as well as save data (if we want to) and terminate
        threads as necessary  etc. """
        
        if self.auto_send_job is not None:
            self.gui.after_cancel(self.auto_send_job)
            self.auto_send_job = None
        
        if self.serial_object:
            self.serial_object.close()
        
        if self.plotObject:
            self.plotObject.close()
                           
        if self.update_job is not None:
            self.gui.after_cancel(self.update_job )
            self.update_job = None
            
        if self.gui:
            self.gui.quit()
            self.gui.destroy()
        self.gui = None
            
    def send(self):
        """This function is for sending data from the computer to the host controller.
        The value entered in the the entry box is pushed to the UART. The data can be of any format, since
        the data is always converted into ASCII, the receiving device has to convert the data into the required f
        format.
        """
        data = []
        data_format = []
        for (combo_box, entry) in zip(self.combobox_out, self.data_entry):
            if entry.get() != '':
                data.append(entry.get())
                data_format.append(self.out_selection[combo_box.current()])
        
        if len(data):
            # Something to send
            self.text.insert(END, ">>> ")
            (sucess, err_msg) = self.serial_object.write(data, data_format)
            if  not sucess: #self.serial_object.write(cmd, float1, float2):
                self.text.insert(END, "SEND ERROR <"+err_msg+">: ")
            
            for v in data:
                self.text.insert(END, " " + str(v) )
                
            self.text.insert(END, "\n")
                
                
    def plotWindowOpenClose(self):
        if self.plotObject is None and self.serial_object.isConnected():
            self.plotObject = serial_monitor_lib.RealTimePlot()
            self.serial_object.registerCallback(self.plotObject.addValue)
            self.plotObject.Start(self.gui)
            self.plot_select.config(state='enabled')
            self.graphing.configure(text="Close Plot")
            self.plot_selection_changed()
            
        elif self.plotObject is not None:
            self.plotObject.close()
            self.serial_object.removeCallback(self.plotObject.addValue)
            self.plotObject = None
            self.plot_select.config(state='disabled')
            self.graphing.configure(text="Open Plot")
        else:
            print("Cannot start graphing until serial is connected.\n")

    def dataRecordingStartStop(self):
        if self.recordObject is None and self.serial_object.isConnected():
            self.recordObject = serial_monitor_lib.RecordData()
            self.serial_object.registerCallback(self.recordObject.addData)
            self.recordObject.startRecording()
            self.recording.configure(text="Stop Recording")
        elif self.recordObject is not None:
            self.recordObject.stopRecording()
            self.serial_object.removeCallback(self.recordObject.addData)
            self.recordObject.saveData()
            self.recordObject = None
            self.recording.configure(text="Start Recording")
        else:
            print("Cannot start recording until serial is connected.\n")
        
    def loadCSV_Commands(self):
        filename = filedialog.askopenfilename(filetypes=(("csv files", "*.csv"), ("all files", "*.*")),defaultextension='.csv')
        if not filename:
            return # none was selected
        
        csvfile = open(filename,newline='\n')
        csv_reader = csv.reader(csvfile, delimiter=',')
        
        command_list = collections.deque()
        for row in csv_reader:
            if len(row) is not 0:
                command_list.append(row)
        
        csvfile.close()
            
        for row in command_list:
            try:
                size = struct.calcsize(row[0])
                if len(row[0]) is not len(row)-1:
                    print("Missmatch Encoding Number and Entries for Command String: " + row )
                    return
            except:
                print(row[0])
                print("Invalid Command Type: " + row[0] )
                return
        
        delay_time = simpledialog.askfloat("Input", "Milliseconds Between Commands?",
                               parent=self.gui,
                               minvalue=0, maxvalue=10000)
        
        self.sendCSV_Commands(command_list, delay_time)
     
    def sendCSV_Commands(self, cmd_list, delay_time):
        cmd = cmd_list.popleft();
        fmt = cmd.pop(0)
        
        #load into boxes
        for (f,c,box,entry) in itertools.zip_longest(fmt,cmd,self.combobox_out, self.data_entry,fillvalue=-1):
            entry.delete(0,'end')
            if f is not -1 and c is not -1:
                box.current(self.out_selection.index(f))
                entry.insert(END,"".join(c.split())) # .join(c.split()) removes any whitespace that might have been in the csv file
                
        self.send()
        
        if len(cmd_list) > 0:
            self.auto_send_job = self.gui.after(int(delay_time), lambda:self.sendCSV_Commands(cmd_list,delay_time))
        else:
            self.auto_send_job = None

    def Callback(self, function):
        self.callbackfunction.append(function)

    def main(self):
        # mainloop
        self.gui.geometry('500x500')
        self.gui.mainloop()


if __name__ == '__main__':
    g = GuiSetup()
    g.main()
