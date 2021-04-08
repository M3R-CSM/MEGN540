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

from inputs_mod import get_gamepad, devices

# FOR THREADING AND MUTEX PROTECTION (used in serial interface primarily) 
from threading import Thread, Lock
import collections  # FOR DEQUEUE USED IN DATA STORAGE AND CALLBACK QUEUES

import time

class MEGN540_GamePadInterface:
    def __init__(self):
        self.is_running = False
        self.thread = None
        self.cb_thread = None
        self.cbk_list = collections.deque()
        self.callback_list_mutex = Lock()
        self.rate_mutex = Lock()
        self.last_send_time = time.perf_counter()
        
        self.gamepad = None
        
        
        self.lin_vel = 0
        self.ang_vel = 0
        self.changed = True
        
    def connect(self):
        if self.is_running is False and len(devices.gamepads) > 0:
            
            if not self.gamepad:
                self.gamepad = devices.gamepads[0]
        
            self.thread = Thread(target=self.event_thread)
            self.is_running = True
            self.thread.start()
            
            self.cb_thread = Thread(target=self.process_callbacks)
            self.cb_thread.start()
            print("Game Pad Event Handler Started")
        else:
            print("Could not connect to gamepad.")
            
    def disconnect(self):
        self.is_running = False
        self.gamepad = None
        
        if self.thread:
            self.thread.join(0.1)
        if self.cb_thread:
            self.cb_thread.join(0.1)
            
        self.thread = None
        self.cb_thread = None
        
        
    def is_connected(self):
        return self.is_running
        
    def add_callback(self, cbk_function):
        self.callback_list_mutex.acquire()
        try:
           self.cbk_list.append(cbk_function)
        finally:
           self.callback_list_mutex.release()
        
    def rm_callback(self, cbk_function):
        self.callback_list_mutex.acquire()
        try:
            self.cbk_list.remove(cbk_function)
        finally:
            self.callback_list_mutex.release()
            
    def rumble_pad(self):
        self.gamepad.set_vibration(1, 0, 1000)
    
    def event_thread(self):
        
        while self.is_running:
             try:
                events = devices.gamepads[0]._do_iter()
                #events = get_gamepad()
                if events is None:
                    time.sleep(0.0001)
                    continue
                
                for event in events:
                    if event.ev_type is "Absolute":
                        try:
                            self.rate_mutex.acquire()
                        
                            if event.code is "ABS_X":
                                #self.ang_vel = -6.28*(event.state-128)/128
                                #self.changed = True
                                continue
                            elif event.code is "ABS_Y":
                                self.lin_vel = -0.5*(event.state-128)/128
                                self.changed = True
                            elif event.code is "ABS_RZ":
                                #self.lin_vel = -0.5*(event.state-128)/128
                                #self.changed = True
                                continue
                            elif event.code is "ABS_Z":
                                self.ang_vel = -6.28*(event.state-128)/128
                                self.changed = True
                        
                        finally:
                            self.rate_mutex.release()
                       
             except:
                 print("Game pad error! Disconnecting")
                 break;
                    
            
                           
                
        self.is_running = False
    
    def process_callbacks(self):
        while self.is_running:
            try:
                self.rate_mutex.acquire()
                if time.perf_counter() - self.last_send_time > 0.1 and self.changed:
                    self.callback_list_mutex.acquire()
                    try:
                        if abs(self.ang_vel) < 0.01:
                            self.ang_vel = 0
                        if abs(self.lin_vel) < 0.01:
                            self.lin_vel = 0
                            
                        for function in self.cbk_list:
                            function(self.lin_vel, self.ang_vel)
                    finally:
                        self.callback_list_mutex.release()
                    
                    self.last_send_time = time.perf_counter()
                    self.changed = False
            finally:
                self.rate_mutex.release()
                
            time.sleep(0.03)
                
                

def print_check(lin,ang):
    print("Lin: " + str(lin) + " Ang: " + str(ang) )
        
def main():
    
    devices.gamepads[0]
    events = get_gamepad()
    
    interface = MEGN540_GamePadInterface()
    interface.add_callback(print_check)
    interface.connect()
    
    while interface.is_connected():
        continue
                
                
if __name__ == "__main__":
    main()