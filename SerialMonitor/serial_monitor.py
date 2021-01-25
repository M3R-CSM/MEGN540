# import tkinter
import multiprocessing
import queue
from tkinter import *      # for gui general
from tkinter.ttk import *  # for combobox
import threading
import time
import serial
import serial_monitor_lib


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
        self.serial_data = multiprocessing.Queue(maxsize=10)

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

        # threads
        self.update_gui_thread = threading.Thread(target=self.update_gui)
        self.update_gui_thread.daemon = True
        self.update_gui_thread.start()

#        # Thread creation for gui text box functionality        
#        self.t3 = threading.Thread(target=self.debug_check_gui)
#        self.t3.daemon = True
#        self.t3.start()



        # Send Button & Input
        xLoc = 15
        yLoc = 227
        self.button1 = Button(text="Send", command=self.send, width=6).place(x=xLoc, y=yLoc)
        xLoc = 100
        yLoc += 5
        self.label_char = Label(text="char").place(x=xLoc+5, y=yLoc-20)
        self.data_entry_char = Entry(self.gui)
        self.data_entry_char.place(x=xLoc, y=yLoc, width=45)
        xLoc += 50
        self.label_float_1 = Label(text="float 1").place(x=xLoc+15, y=yLoc-20)
        self.data_entry_float_1 = Entry(self.gui)
        self.data_entry_float_1.place(x=xLoc, y=yLoc, width=70)
        xLoc += 75
        self.label_float_2 = Label(text="float 2").place(x=xLoc+15, y=yLoc-20)
        self.data_entry_float_2 = Entry(self.gui)
        self.data_entry_float_2.place(x=xLoc, y=yLoc, width=70)


        # Incomming Message Formatting String
        xLoc += 150
        self.charnum = Label(text="Incoming Format").place(x=xLoc-20, y=yLoc-20)
        self.input_format_sv = StringVar()
        self.input_format_sv.trace("w", lambda name, index, mode, sv=self.input_format_sv: self.change_input_format_callback(sv))
        self.format_entry = Entry(width=7, textvariable=self.input_format_sv)
        self.format_entry.place(x=xLoc, y=yLoc)
        self.input_format_sv.set("c")

        # Serial baudrate box
        xLoc = 130
        yLoc = 365
        self.baud = Label(text="Baud").place(x=xLoc, y=yLoc)
        self.baud_entry = Entry(width=7)
        self.baud_entry.place(x=(xLoc+50), y=yLoc)
        self.baud_entry.insert(1,9600)

        # Serial Port Input Box
        yLoc = 400
        self.port = Label(text="Port").place(x=xLoc, y=yLoc)
        self.port_entry = Entry(width=25)
        self.port_entry.place(x=(xLoc+50), y=yLoc)
        self.port_entry.insert(1,"/dev/ttyACM0")

        # button
        self.button_connect = Button(   text=" Connect  ", command=self.connectToSerial)
        self.button_connect.place(x=15, y=360)

        self.graphing = Button(text="Open Plot", command=self.graph)
        self.graphing.place(x=305, y=320)

        self.recording = Button(text="Start Recording", command=self.record)
        self.recording.place(x=185, y=320)

        self.plotObject = None
        self.recordObject = None

    def change_input_format_callback(self, sv):
        self.serial_object.setDataFormat(sv.get())

    def addValue(self, value):
        self.serial_data.put(value)

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
        while self.ok: # self.ok gets set to False on window exit
            try:
                self.text.insert(END, str(self.serial_data.get_nowait()))  # puts text data on monitor
                self.text.insert(END, "\n")
                if self.text.yview()[1] > .9: # make slider stickey if at bottom
                    self.text.yview(END)
            except queue.Empty:
                time.sleep(1/self.text_box_update_Hz) #sleep 1/updateHz
                pass              
            
    def close_window(self):
    """ This function is for some internal cleanup operations on closeing to make sure
        we leave the serialport in a good state as well as save data (if we want to) and terminate
        threads as necessary  etc.
    """
self.ok = False # Tell thread to teminate while loop
        self.update_gui_thread.join()
        
        if self.serial_object:
            self.serial_object.close()
        
        if self.plotObject:
            self.plotObject.close()
            
        if self.recordObject and self.recordObject.isRecording():
            self.recordObject.saveData()
            
        self.gui.destroy()
        
            
    def send(self):
        """This function is for sending data from the computer to the host controller.
        The value entered in the the entry box is pushed to the UART. The data can be of any format, since
        the data is always converted into ASCII, the receiving device has to convert the data into the required f
        format.
        """
        cmd = self.data_entry_char.get()
        
        float1_str = self.data_entry_float_1.get()
        if float1_str:
            try:
                float1 = float(float1_str)
            except ValueError:
                float1 = []
                print("ERROR: Float 1 send box value [" + float1_str+"] is not a valid float")
        else:
            float1 = []
    
        float2_str = self.data_entry_float_2.get()
        if float2_str:
            try:
                float2 = float(float2_str)
            except ValueError:
                float2 = []
                print("ERROR: Float 1 send box value [" + float1_str+"] is not a valid float")
        else:
            float2 = []
                
        if cmd or float1 or float2:
            # Something to send
            if self.serial_object.write(cmd, float1, float2):
                # Print to IO terminal
                self.text.insert(END, ">>> "+ str(cmd) + " " + str(float1) + " " + str(float2) + "\n")
            else:
                self.text.insert(END, ">>> ERROR CANNOT SEND MSG: " + str(cmd) + " " + str(float1) + " " + str(float2) + "\n")
                
                
    def graph(self):
        if self.plotObject is None and self.serial_object.isConnected():
            self.plotObject = serial_monitor_lib.RealTimePlot()
            self.serial_object.registerCallback(self.plotObject.addValue)
            self.plotObject.Start()
        elif self.plotObject is not None:
            self.plotObject.close()
            self.serial_object.removeCallback(self.plotObject.addValue)
            self.plotObject = None
        else:
            print("Cannot start graphing until serial is connected.\n")

    def record(self):
        if self.recordObject is None and self.serial_object.isConnected():
            self.recordObject = serial_monitor_lib.RecordData()
            self.serial_object.registerCallback(self.recordObject.addData)
            self.recordObject.startRecording()
            self.recording.configure(text="Stop Recording")
        elif self.recordObject is not None:
            self.recordObject.stopRecording()
            self.serial_object.removeCallback(self.recordObject.addData)
            self.recordObject = None
            self.recording.configure(text="Start Recording")
        else:
            print("Cannot start recording until serial is connected.\n")
            

    def Callback(self, function):
        self.callbackfunction.append(function)

    def main(self):
        # mainloop
        self.gui.geometry('500x500')
        self.gui.mainloop()


if __name__ == '__main__':
    g = GuiSetup()
    g.main()
