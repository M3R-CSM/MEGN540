#include "Encoder.h"

/**
 * Internal counters for the Interrupts to increment or decrement as necessary.
 */
static volatile bool _last_right_A = 0;  // Static limits it's use to this file
static volatile bool _last_right_B = 0;  // Static limits it's use to this file

static volatile bool _last_left_A   = 0;  // Static limits it's use to this file
static volatile bool _last_left_B   = 0;  // Static limits it's use to this file
static volatile bool _last_left_XOR = 0;  // Necessary to check if PB4 triggered the ISR or not

static volatile int32_t _left_counts  = 0;  // Static limits it's use to this file
static volatile int32_t _right_counts = 0;  // Static limits it's use to this file

/** Helper Funcions for Accessing Bit Information */
// *** MEGN540 Lab 3 TODO ***
// Hint, use avr's bit_is_set function to help
static inline bool Right_XOR()
{
    return 0;
}  // MEGN540 Lab 3 TODO
static inline bool Right_B()
{
    return 0;
}  // MEGN540 Lab 3 TODO
static inline bool Right_A()
{
    return 0;
}  // MEGN540 Lab 3 TODO

static inline bool Left_XOR()
{
    return 0;
}  // MEGN540 Lab 3 TODO
static inline bool Left_B()
{
    return 0;
}  // MEGN540 Lab 3 TODO
static inline bool Left_A()
{
    return 0;
}  // MEGN540 Lab 3 TODO

/**
 * Function Encoders_Init initializes the encoders, sets up the pin change interrupts, and zeros the initial encoder
 * counts.
 */
void Initialize_Encoders()
{
    // *** MEGN540 Lab3 ***
    // YOUR CODE HERE

    // Left encoder uses PB4 and PE2 pins as digital inputs. External interrupt PCINT4 is necessary to detect
    // the change in XOR flag. You'll need to see Section 11.1.5 - 11.1.7 for setup and use.
    // Note that the PCINT interrupt is trigered by any PCINT pin. In the ISR you should check to make sure
    // the interrupt triggered is the one you intend on processing.

    // Right encoder uses PE6 adn PF0 as digital inputs. External interrupt INT6 is necessary to detect
    // the changes in XOR flag. You'll need to see Sections 11.1.2-11.1.4 for setup and use.
    // You'll use the INT6_vect ISR flag.

    // Initialize static file variables. These probably need to be updated.
    _last_right_A = 0;  // MEGN540 Lab 3 TODO
    _last_right_B = 0;  // MEGN540 Lab 3 TODO

    _last_left_A   = 0;  // MEGN540 Lab 3 TODO
    _last_left_B   = 0;  // MEGN540 Lab 3 TODO
    _last_left_XOR = 0;  // MEGN540 Lab 3 TODO

    _left_counts  = 0;  // MEGN540 Lab 3 TODO
    _right_counts = 0;  // MEGN540 Lab 3 TODO
}

/**
 * Function Encoder_Counts_Left returns the number of counts from the left encoder.
 * @return [int32_t] The count number.
 */
int32_t Encoder_Counts_Left()
{
    // *** MEGN540 Lab3 ***
    // YOUR CODE HERE
    // Note: Interrupts can trigger during a function call and an int32 requires
    // multiple clock cycles to read/save. You may want to stop interrupts, copy the value,
    // and re-enable interrupts to prevent this from corrupting your read/write.
    return 0;
}

/**
 * Function Encoder_Counts_Right returns the number of counts from the right encoder.
 * @return [int32_t] The count number.
 */
int32_t Encoder_Counts_Right()
{
    // *** MEGN540 Lab3 ***
    // YOUR CODE HERE
    // Note: Interrupts can trigger during a function call and an int32 requires
    // multiple clock cycles to read/save. You may want to stop interrupts, copy the value,
    // and re-enable interrupts to prevent this from corrupting your read/write.
    return 0;
}

/**
 * Function Encoder_Rad_Left returns the number of radians for the left encoder.
 * @return [float] Encoder angle in radians
 */
float Encoder_Rad_Left()
{
    // *** MEGN540 Lab3 ***
    // YOUR CODE HERE.  How many counts per rotation???
    return 0;
}

/**
 * Function Encoder_Rad_Right returns the number of radians for the left encoder.
 * @return [float] Encoder angle in radians
 */
float Encoder_Rad_Right()
{
    // *** MEGN540 Lab3 ***
    // YOUR CODE HERE.  How many counts per rotation???
    return 0;
}

/**
 * Interrupt Service Routine for the left Encoder. Note: May need to check that it is actually PCINT4 that triggered, as
 * the Pin Change Interrupts can trigger for multiple pins.
 * @return
 */
// ISR()
//{
//
//}

/**
 * Interrupt Service Routine for the right Encoder.
 * @return
 */
// ISR()
//{
//
//}