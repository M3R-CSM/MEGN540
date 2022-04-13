#include "Battery_Monitor.h"

static const float BITS_TO_BATTERY_VOLTS = 0.0f;

/**
 * Function Initialize_Battery_Monitor initializes the Battery Monitor to record the current battery voltages.
 */
void Initialize_Battery_Monitor()
{

    // *** MEGN540 LAB3 YOUR CODE HERE ***
}

/**
 * Function Battery_Voltage initiates the A/D measurement and returns the result for the battery voltage.
 */
float Battery_Voltage()
{
    // A Union to assist with reading the LSB and MSB in the  16 bit register
    union {
        struct {
            uint8_t LSB;
            uint8_t MSB;
        } split;
        uint16_t value;
    } data = { .value = 0 };

    // *** MEGN540 LAB3 YOUR CODE HERE ***

    return data.value * BITS_TO_BATTERY_VOLTS;
}
