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

#ifndef F_CPU
#    define F_CPU 16000000
#endif

#include "led_interface.h"

/**
 * Function blink_string takes a c-string and blinks the morse code corresponding to the characters provided.
 * @param str pointer to a null-germinated c-string object
 * @param dot_length_ms The characteristic length of a dot in the morse-code pattern.
 */
void blink_morse_str( char* str, uint16_t dot_length_ms )
{
    // INFO:  C-Strings are null-terminated (end in zero). You can use this to iterate through a c-string object without
    // having to know its length explicitly (that's the whole point).
    // Specifically: char* foo = "hi yo!"; gets stored as: [ 'h', 'i', ' ', 'y', 'o', '!', 0x00 ]

    // BEGIN FUNCTION blink_string
    // Define a counter variable of type uint8_t
    // DO:
    //      blink pattern for the str[counter] character with a dot-length specified by dot_length_ms
    //      increment the counter
    // WHILE: str[counter] is not null (0) and counter is < 255 (it's always good to work with an infinite-loop net)
    //
    // END FUNCTION
}

int main()
{
    initialize_LED();
    const uint16_t dot_length_ms = 250;

    while( 1 ) {
        // blink message!
        blink_morse_char( 'S', dot_length_ms );
        blink_morse_char( 'O', dot_length_ms );
        blink_morse_char( 'S', dot_length_ms );
        blink_morse_char( ' ', dot_length_ms );

        // Using your blink_morse_str function this would be called as:
        // blink_morse_str("SOS ", dot_length_ms);
    }
}
