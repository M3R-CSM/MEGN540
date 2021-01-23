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
        self.gui.title("window")

        self.update_period = 5
        self.serial_object = serial_monitor_lib.SerialData()
        self.serial_object.registerCallback(self.addValue)
        self.plotThread = None

        self.button_var = IntVar()

        self.version_ = self.button_var.get()  # in connectToSerial

        # frames
        self.frame_1 = Frame(height=285, width=480, relief='groove').place(x=7, y=5)
        self.frame_2 = Frame(height=150, width=480, relief='groove').place(x=7, y=300)
        self.text = Text(width=56, height=10)

        # threads
        self.t2 = threading.Thread(target=self.update_gui)
        self.t2.daemon = True
        self.t2.start()


        # Contact Info
        self.contact = Label(text="apetruska@mines.com").place(x=250, y=437)

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

        # self.os_combobox = Combobox(self.gui, values=['Linux', 'Windows'],width=7)
        # self.os_combobox.place(x=15, y=320)
        # self.os_combobox.current(0)
        # #os_combobox.pack()


        # button
        self.button_connect = Button(   text=" Connect  ", command=self.connectToSerial)
        self.button_connect.place(x=15, y=360)

        self.graphing = Button(text="Open Plot", command=self.graph)
        self.graphing.place(x=305, y=320)

        self.recording = Button(text="Start Recording", command=self.record)
        self.recording.place(x=185, y=320)

        self.serial_data = multiprocessing.Queue(maxsize=2)

        self.plotObject = None
        self.recordObject = None

    def change_input_format_callback(self, sv):
        self.serial_object.setDataFormat(sv.get())

    def addValue(self, value):
        if self.serial_data != None:
            try:
                self.serial_data.get_nowait()
            except queue.Empty:
                pass

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
            self.version_ = self.button_var.get()  # in connectToSerial

            print(self.version_)
            '''self.serial_object.append(baud)
            self.serial_object.append(port)'''
            # global serial_object
            self.port = self.port_entry.get()
            self.baud = self.baud_entry.get()

            try:
                self.serial_object.openPort( str(self.port), self.baud)
            except ValueError:
                print("Can't Open Specified Port")
        else:  # Disconnect
            try:
                self.serial_object.close()
            except AttributeError:
                print("Closed without using it -_-")

            if self.plotThread is not None:
                serial_monitor_lib.close()
                self.plotThread.join()

        if self.serial_object.isConnected():
            self.button_connect.configure(text="Disconnect")
        else:
            self.button_connect.configure(text=" Connect ")


            # try:
            #     if self.version_ == 2:
            #         try:
            #             self.serial_object.openPort('/dev/tty' + str(self.port), self.baud)
            #         except:
            #             print("Can't Open Specified Port")
            #     elif self.version_ == 1:
            #         self.serial_object.openPort('COM' + str(self.port), self.baud)
            # except ValueError:
            #     print("Enter Baud and Port")
            #     return

    def update_gui(self):
        """" This function is an update function which is also threaded. The function assimilates the data
        and applies it to it corresponding progress bar. The text box is also updated every couple of seconds.
        A simple auto refresh function .after() could have been used, this has been avoid purposely due to various
        performance issues.
        """
        # global filter_data TODO
        # global update_period TODO
        # self.filter_data = serial_monitor_lib.SerialData.parseData(self).value,


        self.text.place(x=15, y=10)
        # progress_1.place(x=60, y=100)
        # progress_2.place(x=60, y=130)
        # progress_3.place(x=60, y=160)
        # progress_4.place(x=60, y=190)
        # progress_5.place(x=60, y=220)
        new = time.time()

        while 1:
            # print(len(self.filter_data))
            try:
                self.text.insert(END, str(self.serial_data.get_nowait()))  # puts text data on monitor
                self.text.insert(END, "\n")
                if time.time() - new >= self.update_period:
                    self.text.delete("1.0", END)
                    # progress_1["value"] = 0
                    # progress_2["value"] = 0
                    # progress_3["value"] = 0
                    # progress_4["value"] = 0
                    # progress_5["value"] = 0
                    new = time.time()
            except queue.Empty:
                pass

    def send(self):
        """This function is for sending data from the computer to the host controller.
        The value entered in the the entry box is pushed to the UART. The data can be of any format, since
        the data is always converted into ASCII, the receiving device has to convert the data into the required f
        format.
        """
        self.send_data = self.data_entry_char.get()

        if not self.send_data:
            print("Sent Nothing")
        # self.serial_object.write(self.send_data.encode('utf-8'))
        self.serial_object.write(self.send_data.encode('utf-8'));

        try:
            if self.button_var.get() == 3:
                print(self.data_entry_char.get() + self.terminator_entry)
        except ValueError:
            print("Enter Baud and Port")
            return

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



    # def disconnect(self):
    #     """
    #     This function is for disconnecting and quitting the application. Sometimes the application throws a couple of
    #     errors while it is being shut down, the fix isn't out yet but will be pushed to the repo once done. simple
    #     GUI.quit() calls.
    #     """
    #     try:
    #         self.serial_object.close()
    #     except AttributeError:
    #         print("Closed without using it -_-")
    #
    #     if self.plotThread != None:
    #         serial_monitor_lib.close()
    #         self.plotThread.join()
    #     self.gui.quit()

    def Callback(self, function):
        self.callbackfunction.append(function)

    def main(self):
        # mainloop
        self.gui.geometry('500x500')
        self.gui.mainloop()


if __name__ == '__main__':
    g = GuiSetup()
    g.main()
