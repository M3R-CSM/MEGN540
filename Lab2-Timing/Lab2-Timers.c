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

#include "MEGN540_Lab_Tasks.h"
#include "MEGN540_Message_Handeling.h"  // for translating USB messages to microcontroller tasks
#include "SerialIO.h"                   // for USB communication
#include "Task_Management.h"            // for clean task management with functors
#include "Timing.h"                     // for Time understanding

// Add these Lab-Specific tasks functions to MEGN540_Lab_Tasks.h/c so you can link to tasks in the
// initialization function...
// void Task_Send_LoopTime(float _time_since_last);
// void Task_Send_TimeNow(float _time_since_last);

// put your global variables (only if absolutely required) here.
// Best to identify them as static and start with a _ to make them indentified as internal.
// Try to initialize them if possible, so their values are never arbitrary.

/**
 * Function Initialize_Modules sets up all hardware and persistant memory necessary
 * for the main loop to funciton properly. It is the first thing main should call and is
 * a convenient way or resetting the system if that is requested.
 *
 * It takes a float as a parameter that is not used to allow it to integrate nicely with the Task_Management Code (which should pass it the seconds since last
 * ran)
 *
 */
void Initialize_Modules( float _time_not_used_ )
{
    // Initialize (reinitialize) all global variables

    // Initialize all modules
    Initialize_USB();
    Initialize_Timing();

    // Setup task handling
    Initialize_Task( &task_restart, -1 /*do only once*/, Initialize_Modules /*function pointer to call*/ );
}

/** Main program entry point. This routine configures the hardware required by the application, then
 *  enters a loop to run the application tasks in sequence.
 */
int main( void )
{
    Initialize_Modules( 0.0 );

    for( ;; ) {
        USB_Upkeep();
        MEGN540_Message_Handling_Upkeep();

        Task_Run_If_Ready( &task_restart );
    }
}
