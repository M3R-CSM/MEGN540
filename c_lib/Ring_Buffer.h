/*
         MEGN540 Mechatronics Lab
    Copyright (C) Andrew Petruska, 2023.
       apetruska [at] mines [dot] edu
          www.mechanical.mines.edu
*/

/*
    Copyright (c) 2023 Andrew Petruska at Colorado School of Mines

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

/* Ring_Buffer.h
 *
 * This set of functions enables a ringbuffer for both float (F) and uint8_t/byte (B) data types.
 * A ring buffer allows constant data addition and removal in a fixed size array.
 * The ring buffer will overwrite existing elements of the array if more data is
 * added than there is adequate space. This works well for a First in First Out
 * or Last in First Out type queue.
 *
 * Functions implemented are as follows (where X is either F or B to denote float or uint8_t/byte):
 *
 * Ring_Buffer_Float_t  or Ring_Buffer_Byte_t   <-- The internal data structure for the ringbuffer object
 * rb_print_data_X  <-- Prints debugging information to the terminal assist with code generation and capabilities
 * rb_initialize_X  <-- Initializes the ring buffer for use.
 * rb_length_X      <-- Returns the number of active elements in the ringbuffer
 * rb_push_back_X   <-- Appends an element to the end of the buffer
 * rb_push_front_X  <-- Appends an element to the start of the buffer
 * rb_pop_back_X    <-- Removes and returns the last element
 * rb_pop_front_X   <-- Removes and returns the first element
 * rb_get_X         <-- Returns an desired element from within the buffer
 * rb_set_X         <-- Sets a desired element within the buffer
 *
 * Code Skeleton provided by Dr Petruska for MEGN 540, Mechatronics
 * Code Details Provided by:  [ YOUR NAME ]
 * Code Last Modified:  1/24/2023
 * */
#ifndef RING_BUFFER_H
#define RING_BUFFER_H

#include "stdint.h"  // for uint8_t type

#ifndef RB_LENGTH_F
#    define RB_LENGTH_F 8  // must be a power of 2 (max of 256). This is an easy place to adjust max expected length
#endif

#ifndef RB_LENGTH_B
#    define RB_LENGTH_B 16  // must be a power of 2 (max of 256). This is an easy place to adjust max expected length
#endif

// data structure for a float ring buffer
typedef struct {
    float buffer[RB_LENGTH_F];
    uint8_t start_index;
    uint8_t end_index;
} Ring_Buffer_Float_t;

// data structure for a uint8_t ring buffer
typedef struct {
    uint8_t buffer[RB_LENGTH_B];
    uint8_t start_index;
    uint8_t end_index;
} Ring_Buffer_Byte_t;

#ifndef AVR_MCU  // dont build these for the car builds
// Debugging Assistant Functions (these are already written for you)
void rb_print_data_F( Ring_Buffer_Float_t* p_buf );
void rb_print_data_B( Ring_Buffer_Byte_t* p_buf );
#endif

/****** Functions   **********/

/* Initialization */
void rb_initialize_F( Ring_Buffer_Float_t* p_buf );
void rb_initialize_B( Ring_Buffer_Byte_t* p_buf );

/* Return active Length of Buffer */
uint8_t rb_length_F( const Ring_Buffer_Float_t* p_buf );
uint8_t rb_length_B( const Ring_Buffer_Byte_t* p_buf );

/* Append element to end and lengthen */
void rb_push_back_F( Ring_Buffer_Float_t* p_buf, float value );
void rb_push_back_B( Ring_Buffer_Byte_t* p_buf, uint8_t value );

/* Append element to front and lengthen */
void rb_push_front_F( Ring_Buffer_Float_t* p_buf, float value );
void rb_push_front_B( Ring_Buffer_Byte_t* p_buf, uint8_t value );

/* Remove element from end and shorten */
float rb_pop_back_F( Ring_Buffer_Float_t* p_buf );
uint8_t rb_pop_back_B( Ring_Buffer_Byte_t* p_buf );

/* Remove element from start and shorten */
float rb_pop_front_F( Ring_Buffer_Float_t* p_buf );
uint8_t rb_pop_front_B( Ring_Buffer_Byte_t* p_buf );

/* access element */
float rb_get_F( const Ring_Buffer_Float_t* p_buf, uint8_t index );
uint8_t rb_get_B( const Ring_Buffer_Byte_t* p_buf, uint8_t index );

/* set element - This behavior is
   poorly defined if index is outside of active length.
   Use of the push_back or push_front methods are preferred.
*/
void rb_set_F( Ring_Buffer_Float_t* p_buf, uint8_t index, float value );
void rb_set_B( Ring_Buffer_Byte_t* p_buf, uint8_t index, uint8_t value );

#endif
