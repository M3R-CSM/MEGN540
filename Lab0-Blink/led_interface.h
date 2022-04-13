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

#include <avr/io.h>
#include <util/delay.h>

#include <ctype.h>   // tolower()
#include <stdint.h>  // for uint32_t type definition

#ifndef LAB0_BLINK_LED_INTERFACE_H
#    define LAB0_BLINK_LED_INTERFACE_H

#    define initialize_LED() DDRC |= ( 1 << DDC7 )  // Make pin 13 be an output.

#    define LED_ON() PORTC |= ( 1 << PORTC7 )  // Turn the LED on.

#    define LED_TOGGLE() PORTC ^= ( 1 << PORTC7 )  // Toggles the led state

#    define LED_OFF() PORTC &= ~( 1 << PORTC7 )  // Turn the LED off.

void led_on_block( uint32_t ms );
void led_off_block( uint32_t ms );

void blink_morse_char( char value, uint32_t unit_length );
void morse_new_letter( uint32_t unit_length );

/** helper functions **/
void morse_dot( uint32_t unit_length );
void morse_dash( uint32_t unit_length );
void dot_dash_blink( char* c_str, uint32_t unit_length );

#endif  // LAB0_BLINK_LED_INTERFACE_H
