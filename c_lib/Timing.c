/*
  Copyright 2020  Andrew Petruska (apetruska [at] mines [dot] edu)

  Permission to use, copy, modify, distribute, and sell this
  software and its documentation for any purpose is hereby granted
  without fee, provided that the above copyright notice appear in
  all copies and that both that the copyright notice and this
  permission notice and warranty disclaimer appear in supporting
  documentation, and that the name of the author not be used in
  advertising or publicity pertaining to distribution of the
  software without specific, written prior permission.

  The author disclaims all warranties with regard to this
  software, including all implied warranties of merchantability
  and fitness.  In no event shall the author be liable for any
  special, indirect or consequential damages or any damages
  whatsoever resulting from loss of use, data or profits, whether
  in an action of contract, negligence or other tortious action,
  arising out of or in connection with the use or performance of
  this software.
*/

#include "../c_lib/Timing.h"


/** These define the internal counters that will be updated in the ISR to keep track of the time
 *  The volatile keyword is because they are changing in an ISR, the static means they are not
 *  visible (not global) outside of this file.
 */
static volatile uint16_t _count_sec = 0;  // 18 hours till overflow
static volatile uint16_t _count_ms = 0;

/**
 * Function SetupTimer0 initializes Timer0 to have a prescalar of XX and initializes the compare
 * feature for use in an ISR.  It also enables ISR's.
 *
 *  FCPU = 16 000 000 Hz,  1 us => 1 000 000 Hz,  A prescalar of 16 would give you millisecond resolution, but you dont
 *  have enough time to do those additions every microsecond, in addition the 8-bit timer will roll over every 256 microseconds.
 *  So, you need to be counting in (effectively) 4us increments.  So use a prescalar of 64. This will give you a resolution of 4us per
 *  Timer0 Bit and enough head-room to compare at with 249 for a 1kHz update.
 *
 *  Since Timer 0 will be triggering at a kHz, we may want other things to be updated at 1kHz too.
 *
 */
void SetupTimer0()
{
    // YOUR CODE HERE


    ms_counter_1 = 0;
    ms_counter_2 = 0;
    ms_counter_3 = 0;
    ms_counter_4 = 0;
}

/**
 * This function gets the current time and returns it in a Time_t structure.
 * @return
 */
Time_t GetTime()
{
    Time_t time ={
                    .seconds = _count_sec,
                    .millisec = _count_ms,
                    .microsec = 0 // YOU NEED TO REPLACE THIS WITH A CALL TO THE TIMER0 REGISTER AND MULTIPLY APPROPRIATELY
                 };

    return time;
}

/**
 * These functions return the individual parts of the Time_t struct, useful if you only care about
 * things on second or millisecond resolution.
 * @return
 */
uint16_t GetSec()
{
    return _count_sec;
}
uint16_t GetMilli()
{
    return _count_ms;
}
uint16_t GetMicro()
{
    return 0 ;// YOU NEED TO REPLACE THIS WITH A CALL TO THE TIMER0 REGISTER AND MULTIPLY APPROPRIATELY
}

/**
 * This function takes a time struct and returns a float32 value of the equivalent seconds.
 * @param p_time a pointer to a Time structure object
 * @return float32 second equivalent
 */
float  ConvertToFloat( const Time_t* time_p)
{
    // YOUR CODE HERE
    return 0;
}

/**
 * This function takes a start time and calculates the time since that time, it returns it in the Time struct.
 * @param p_time_start a pointer to a start time struct
 * @return (Time_t) Time since the other time.
 */
Time_t  TimeSince(const Time_t* time_start_p )
{
    // YOUR CODE HERE. Dont just convert to float, its faster to do integer addition/subtraction.
    // Be careful of rounding!
    Time_t ret_val = {
                        .seconds = 0,
                        .millisec = 0,
                        .microsec = 0
                     };
    return ret_val;
}

/** This is the Interrupt Service Routine for the Timer0 Compare A feature.
 * You'll need to set the compare flags properly for it to work.
 */
/*ISR( DEFINE THE COMPARISON TRIGGER )
{
    // YOU NEED TO RESET THE Timer0 Value to 0 again!

    // take care of upticks of both our internal and external variables.
    _count_ms ++;
    if( _count_ms == 1000 ){
        _count_ms = 0;
        _count_sec ++;
    }
    ms_counter_1 ++;
    ms_counter_2 ++;
    ms_counter_3 ++;
    ms_counter_4 ++;
}*/
