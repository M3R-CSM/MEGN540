/*
         MEGN540 Mechatronics Lab
    Copyright (C) Andrew Petruska, 2021.
       apetruska [at] mines [dot] edu
          www.mechanical.mines.edu
*/

/*
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

*/

#include "Lab1_Tasks.h"
#include "Message_Handling.h"
#include "SerialIO.h"
#include "Task_Management.h"

/**
 * Function Initialize_Modules sets up all hardware and persistant memory necessary
 * for the main loop to funciton properly. It is the first thing main should call and is
 * a convenient way or resetting the system if that is requested.
 *
 */
void Initialize_Modules( float _not_used_ )
{
    // reset USB input buffers
    USB_Flush_Input_Buffer();

    // Initialize Tasks and their associated funciton connections
    Initialize_Task( &task_restart, Initialize_Modules );

    // once you have everythign else working  you can setup the message handling task to be managed by our task management
    // Initialize_Task( &task_message_handling, 0, Task_Message_Handling );
}

/** Main program entry point. This routine configures the hardware required by the application, then
 *  enters a loop to run the application tasks in sequence.
 */
int main( void )
{
    Initialize_USB();
    Initialize_Modules( 0.0 );

    //

    while( true ) {
        Task_USB_Upkeep();  // This we need to run Always and fast, so no need to wrap it with task management

        Task_USB_Echo();  // you'll want to remove this once you get your serial sorted
        // Task_Message_Handling(0.0); // you'll want to uncomment once you get your serial sorted.
        // Instead of above, once you have Task_Message_Handling working, you can setup the message handling task to be managed by our task management
        // Task_Run_If_Ready( &task_message_handling);

        // Below here you'll process state-machine flags.
        Task_Run_If_Ready( &task_restart );
    }
}
