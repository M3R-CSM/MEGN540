//
// Created by apetruska on 1/25/21.
//

#include <avr/io.h>
#include <util/delay.h>

#include <stdint.h> // for uint32_t type definition
#include <ctype.h> // tolower()

#ifndef LAB0_BLINK_LED_INTERFACE_H
#define LAB0_BLINK_LED_INTERFACE_H

#define initialize_LED()  DDRC |= (1 << DDC7)    // Make pin 13 be an output.

#define LED_ON()      PORTC |= (1 << PORTC7)   // Turn the LED on.

#define LED_TOGGLE()  PORTC ^= (1 << PORTC7)   // Toggles the led state

#define LED_OFF()     PORTC &= ~(1 << PORTC7)  // Turn the LED off.


void led_on_block( uint32_t ms );
void led_off_block( uint32_t ms);


void blink_morse_char(char value, uint32_t unit_length);
void morse_new_letter(uint32_t unit_length);

/** helper functions **/
void morse_dot(uint32_t unit_length);
void morse_dash(uint32_t unit_length);
void dot_dash_blink(char* c_str, uint32_t unit_length);





#endif //LAB0_BLINK_LED_INTERFACE_H
