/*
         MEGN540 Mechatronics Lab
    Copyright (C) Andrew Petruska, 2022.
       apetruska [at] mines [dot] edu
          www.mechanical.mines.edu
*/

/*
    Copyright (c) 2022 Andrew Petruska at Colorado School of Mines

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

*/

#include "SerialIO.h"
#include "Timing.h"
#include "MEGN540_MessageHandeling.h"

// Declare Lab-Specific tasks functions here so you can link to tasks in the
// initialization function... 
// remember the static keyword allows data to persist between function calls without
// requiring global scopings
// void Task_Send_LoopTime();
// void Task_Send_TimeNow();

/**
 * Function Initialize_Modules sets up all hardware and persistant memory necessary
 * for the main loop to funciton properly. It is the first thing main should call and is
 * a convenient way or resetting the system if that is requested. 
 * 
 */
void Initialize_Modules()
{
    Initialize_USB(); 
    Initialize_Timing();

    // Setup task handling
    Initialize_Task( &task_restart, -1 /*do only once*/, Initialize_Modules  /*function pointer to call*/);

}


/** Main program entry point. This routine configures the hardware required by the application, then
 *  enters a loop to run the application tasks in sequence.
 */
int main(void)
{
    Initialize_Modules();

    for (;;)
    {
        Task_USB_Upkeep();
        Task_Message_Handling();

        Task_Run_If_Ready( &task_restart );
    }
}


// Fill in lab-task definitons 
// void Task_Send_LoopTime()
//{
//    static Time_t time_start;
//    IF time start is 0,0 set to current time
//    ELSE calculate time since, then set to 0,0 and send usb message
//    ...
//    return;
//}

// void Task_Send_TimeNow()
//{
//    ...    
//    return;
//}