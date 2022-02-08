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

/**
 * Encoder.h/c defines the functions necessary to interface with the optical encoders on the Pololu Zumo 32U4 car.
 * For information regarding external interrupts please consult Section 11 of the atmega32U4 datasheet
 * https://ww1.microchip.com/downloads/en/DeviceDoc/Atmel-7766-8-bit-AVR-ATmega16U4-32U4_Datasheet.pdf
 *
 * The Left encoder XOR for the Zumo Car is attached to Pin Change Interrupt 4 (pin PB4) (Section 11.1.5-11.1.7), the
 * channel A signal is connected to PE2. The Right encoder XOR is attached to External Interrupt 6 (pin PE6), the
 * channel A signal is connected to PF0. The full state can be recovered by remembering the channel A signal and XOR'ing
 * if it changes to define if the B signal side should be updated.
 *
 */
#ifndef _LAB3_ENCODER_H
#define _LAB3_ENCODER_H

#include <avr/interrupt.h> // For Interrupts
#include <avr/io.h>        // For pin input/output access
#include <ctype.h>         // For int32_t type
#include <math.h>          // for M_PI
#include <stdbool.h>       // for bool type

/**
 * Function Initialize_Encoders initializes the encoders, sets up the pin change interrupts, and zeros the initial encoder
 * counts.
 */
void Initialize_Encoders();

/**
 * Function Counts_Left returns the number of counts from the left encoder.
 * @return [int32_t] The count number.
 */
int32_t Encoder_Counts_Left();

/**
 * Function Counts_Right returns the number of counts from the right encoder.
 * @return [int32_t] The count number.
 */
int32_t Encoder_Counts_Right();

/**
 * Function Rad_Left returns the number of radians for the left encoder.
 * @return
 */
float Encoder_Rad_Left();

/**
 * Function Rad_Right returns the number of radians for the left encoder.
 * @return
 */
float Encoder_Rad_Right();


#endif