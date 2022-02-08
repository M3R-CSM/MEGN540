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
 * Timing.h/c defines a time interface using the atmel's 32U4 architecture.
 * For information on the timers please refer to the product documentation:
 * https://ww1.microchip.com/downloads/en/DeviceDoc/Atmel-7766-8-bit-AVR-ATmega16U4-32U4_Datasheet.pdf
 * Section: 13. 8-bit Timer/Counter0 with PWM (Page 94) in the Atmel atmega32U4 datasheet.
 *
 * This will count time in 4us increments and provide an ISR at 1kHz using the A compare.  As
 * Timer 0 has two compare capabilities, the B can be used elsewhere, but note that this class
 * will be resetting it every time it reaches 249.
 *
 */
#ifndef LAB2_TIMING_TIMING_H
#define LAB2_TIMING_TIMING_H

#include <avr/io.h>         // Board Specific pin definations
#include <avr/interrupt.h>  // for interrupt service routine use

#include <ctype.h>


/**
 * Function SetupTimer0 initializes Timer0 to have a prescalar of XX and initializes the compare
 * feature for use in an ISR.  It also enables ISR's.
 */
void Initialize_Timing( );

/**
 * These functions return the individual parts of the Time_t struct, useful if you only care about
 * things on second or millisecond resolution.
 * @return
 */
uint32_t Timing_Get_Milli();
uint16_t Timing_Get_Micro();

/**
 * Struct Time_t captures the time based in milliseconds at a 4-microsecond resolution.
 */
typedef struct { uint32_t millisec; uint16_t microsec; } Time_t;

/**
 * This function gets the current time and returns it in a Time_t structure.
 * @return
 */
Time_t Timing_Get_Time();
float  Timing_Get_Time_Sec();

/**
 * This function takes a start time and calculates the time since that time, it returns it in the Time struct.
 * @param p_time_start a pointer to a start time struct
 * @return (Time_t) Time since the other time.
 */
float  Timing_Seconds_Since(const Time_t* time_start_p );

#endif //LAB2_TIMING_TIMING_H
