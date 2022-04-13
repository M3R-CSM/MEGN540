/*
         MEGN540 Mechatronics Lab
    Copyright (C) Andrew Petruska, 2022.
       apetruska [at] mines [dot] edu
          www.mechanical.mines.edu
*/

/*
    Copyright (c) 2022 Andrew Petruska at Colorado School of Mines

    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"), to
   deal in the Software without restriction, including without limitation the
   rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
   sell copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in
   all copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
   FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
   IN THE SOFTWARE.

*/

/*
 * This file defines a skid-steer controller for the zumo-car. This system
 * will take in a desired linear and angular velocity for the car, convert these
 * to desired left and right side wheel speeds, and have the controller update.
 */
#include "Controller.h"

typedef struct {
    Controller_t controller_left;   // Z-Transform Controller for the left-side drive
    Controller_t controller_right;  // Z-Transform Controller for the right-side drive

    float wheel_base_width;             // The left-to-right seporation distance between the two side's drive wheels
    float conversion_speed_to_control;  // Relates the linear tangential speed of the left-side or right-side wheel to the motor speed being controlled
    float max_abs_control;              // The maximum control that can be applied, to enable control saturation

    float ( *measurement_left_fcn_ptr )( void );   // function pointer to a function that provides a measurement for the left-side's drive angular measurement
    float ( *measurement_right_fcn_ptr )( void );  // function pointer to a function that provides a measurement for the right-side's drive angular measurement

    void ( *control_left_fcn_ptr )( float );   // function pointer to a function that alows a left-side drive control to be specified
    void ( *control_right_fcn_ptr )( float );  // function pointer to a function that alows a right-side drive control to be specified

} Skid_Steer_Controller_t;

void Initialize_Skid_Steer( Skid_Steer_Controller_t* p_skid_steer_cntr, float wheel_base_width, float conversion_speed_to_control, float max_abs_control,
                            float ( *measurement_left_fcn_ptr )( void ), float ( *measurement_right_fcn_ptr )( void ), void ( *control_left_fcn_ptr )( float ),
                            void ( *control_right_fcn_ptr )( float ) );

/**
 * @brief Skid_Steer_Command_Displacement sets a new target diplacment for the robot to execute. This is a relative displacment to the current position, not an
 * absolute target.
 *
 * @param linear The arc-length to travel (m)
 * @param angular The angle to rotate (rad)
 */
void Skid_Steer_Command_Displacement( float linear, float angular );

/**
 * @brief Skid_Steer_Command_Velocity sets a target velocity for the skid-steer system to execute
 *
 * @param linear The tangential velocity to move at (m/s)
 * @param angular The angular rate to spin at (rad/s)
 */
void Skid_Steer_Command_Velocity( float linear, float angular );

/**
 * @brief Skid_Steer_Control_Update executes a control update by comparing current measurments desired values and implements new control setpoints.
 *
 */
void Skid_Steer_Control_Update( float ellapsed_time );
