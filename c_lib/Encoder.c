#include "Encoder.h"

/**
 * Internal counters for the Interrupts to increment or decrement as necessary.
 */
static volatile int32_t _left_counts;
static volatile int32_t _right_counts;

/**
 * Function Encoders_Init initializes the encoders, sets up the pin change interrupts, and zeros the initial encoder
 * counts.
 */
void Encoders_Init()
{
    // *** MEGN540 Lab3 ***
    // YOUR CODE HERE
}


/**
 * Function Counts_Left returns the number of counts from the left encoder.
 * @return [int32_t] The count number.
 */
int32_t Counts_Left()
{
    // *** MEGN540 Lab3 ***
    // YOUR CODE HERE
    // Note: Interrupts can trigger during a function call and an int32 requires
    // multiple clock cycles to read/save. You may want to stop interrupts, copy the value,
    // and re-enable interrupts to prevent this from corrupting your read/write.
    return 0;
}

/**
 * Function Counts_Right returns the number of counts from the right encoder.
 * @return [int32_t] The count number.
 */
int32_t Counts_Right()
{
    // *** MEGN540 Lab3 ***
    // YOUR CODE HERE
    // Note: Interrupts can trigger during a function call and an int32 requires
    // multiple clock cycles to read/save. You may want to stop interrupts, copy the value,
    // and re-enable interrupts to prevent this from corrupting your read/write.
    return 0;
}

/**
 * Function Rad_Left returns the number of radians for the left encoder.
 * @return [float] Encoder angle in radians
 */
float Rad_Left()
{
    // *** MEGN540 Lab3 ***
    // YOUR CODE HERE
    return 0;
}

/**
 * Function Rad_Right returns the number of radians for the left encoder.
 * @return [float] Encoder angle in radians
 */
float Rad_Right()
{
    return 0;
}

/**
 * Interrupt Service Routine for the left Encoder. Note: May need to check that it is actually PCINT4 that triggered, as
 * the Pin Change Interrupts can trigger for multiple pins.
 * @return
 */
//ISR()
//{
//    static volatile bool _last_left_A = ;     // initialization only happens on first call because it's static
//    static volatile bool _last_left_B = 0;    // initialization only happens on first call because it's static
//}


/**
 * Interrupt Service Routine for the right Encoder. Note: May need to check that it is actually PCINT4 that triggered, as
 * the Pin Change Interrupts can trigger for multiple pins.
 * @return
 */
//ISR()
//{
//    static volatile bool _last_right_A = ;     // initialization only happens on first call because it's static
//    static volatile bool _last_right_B = 0;    // initialization only happens on first call because it's static
//}