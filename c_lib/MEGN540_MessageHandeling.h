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

/*
 * This file defines the incomming message length and other details.
 */

#include "SerialIO.h"

/** Message Driven State Machine Flags */
typedef struct { bool active; } MSG_FLAG_t;
typedef struct { bool active; float duration } MSG_FLAG_D_t;
typedef struct { bool active; float duration; uint8_t value }   MSG_FLAG_D_ValueU8_t;
typedef struct { bool active; float duration; int16_t value }   MSG_FLAG_D_ValueI16_t;
typedef struct { bool active; float duration; float value }     MSG_FLAG_D_ValueF_t;
typedef struct { bool active; float duration; float value_1; float value_2 } MSG_FLAG_D_ValueFF_t;

MSG_FLAG_t mf_restart;  ///<-- This flag indicates that the device received a restart command from the hoast. Default inactive.

/**
 * Function Message_Handling_Init initializes the message handeling and all associated state flags and data to their default
 * conditions.
 */
void Message_Handling_Init();

/**
 * Function Message_Handler processes USB messages as necessary and sets status flags to control the flow of the program.
 * It returns true unless the program receives a reset message.
 * @return
 */
void Message_Handling_Task();

/**
 * Function MEGN540_Message_Len returns the number of bytes associated with a command string per the
 * class documentation;
 * @param cmd
 * @return Size of expected string. Returns 0 if unreconized.
 */
uint8_t MEGN540_Message_Len( char cmd );