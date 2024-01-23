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

#ifndef Task_Management_H
#define Task_Management_H

#include "Timing.h"  // defines Time_t struct and timing iterface

#include <stdbool.h>

typedef struct {
    bool is_active;
    float run_period;
    Time_t time_last_ran;
    void ( *task_fcn_ptr )( float );
} Task_t;

/**
 * @brief Function Initialize_Task sets up a task object. Initializes time_last_ran to 0, is_active to false, and run_period to -1.
 *
 * @param task is a pointer to the task object of interest
 * @param task_fcn_ptr is the function pointer to the task function to execute akin to void Task_Foo(float), it will be passed the time since last run. If this
 * pointer is NULL, the function call will be ignored.
 */
void Initialize_Task( Task_t* task, void ( *task_fcn_ptr )( float ) );

/**
 * @brief Function Task_Activate sets the state of is_active to true, sets the run_period as described, and resets the time last ran to the current time.
 * Repeated calls to Task_Activate on an active task act like snoozing a watchdog task timer.
 *
 * @param task is a pointer to the task object of interest
 * @param run_period is the period (in seconds) the task should repeat at. If -1, the task should only be run once.
 */
void Task_Activate( Task_t* task, float run_period );

/**
 * @brief Function Task_ReActivate sets the state of is_active to true and resets the time last ran to the current time.
 * This has the same functionality as Task_Activate but it does not allow changing the tasks's run period.
 *
 * @param task is a pointer to the task object of interest
 */
void Task_ReActivate( Task_t* task );

/**
 * @brief Function Task_Cancel sets the state of is_active to false
 *
 * @param task is a pointer to the task object of interest
 */
void Task_Cancel( Task_t* task );

/**
 * @brief Function Task_Is_Ready checks to see if the given task is ready for execution.
 *
 * @param task pointer to the task object of interest
 * @return true if the run_period has elapsed ince time_last_ran and the task is active
 * @return false if the task is not active or the run_period has not ellapsed
 */
bool Task_Is_Ready( Task_t* task );

/**
 * @brief Function Task_Run runs the task-function (if not NULL) and updates the time last ran to the current time. If the run_period is -1, this also
 * deactivates the task.
 *
 * @param task pointer to the task object of interest
 */
void Task_Run( Task_t* task );

/**
 * @brief Function Task_Run_If_Ready checks to see if the given task is ready for execution, executes the task by passing it the time
 *  ellapsed since it was last run, and resets the time_last_ran appropriately. If the task function pointer is NULL then it just
 *  returns if the task is ready and resets the time_last_ran.
 *
 * @param task
 * @return true if the task was executed
 * @return false if the task was not exectued (because it was not ready)
 */
bool Task_Run_If_Ready( Task_t* task );

#endif