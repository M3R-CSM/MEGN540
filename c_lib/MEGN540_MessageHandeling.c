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

#include "MEGN540_MessageHandeling.h"


/**
 * Function Message_Handling_Init initializes the message handeling and all associated state flags and data to their default
 * conditions.
 */
void Message_Handling_Init()
{
    // *** MEGN540  ***
    // YOUR CODE HERE. This is where you'd initialize any
    // state machine flags to control your main-loop state machine

    mf_restart.active = false; // needs to be initialized to the default values.
    return;
}

/**
 * Function Message_Handler processes USB messages as necessary and sets status flags to control the flow of the program.
 * It returns true unless the program receives a reset message.
 * @return
 */
void Message_Handling_Task()
{
    // *** MEGN540  ***
    // YOUR CODE HERE. I suggest you use your peak function and a switch interface
    return true;
}



/**
 * Function MEGN540_Message_Len returns the number of bytes associated with a command string per the
 * class documentation;
 * @param cmd
 * @return Size of expected string. Returns 0 if unreconized.
 */
uint8_t MEGN540_Message_Len( char cmd )
{
    switch(cmd)
    {
        case '~': return	1; break;
        case '*': return	9; break;
        case '/': return	9; break;
        case '+': return	9; break;
        case '-': return    9; break;
        case 't': return	6; break;
        case 'T': return	2; break;
        case 'e': return	2; break;
        case 'E': return	2; break;
        case 'a': return	1; break;
        case 'A': return 	5; break;
        case 'w': return	1; break;
        case 'W': return 	5; break;
        case 'm': return	1; break;
        case 'M': return	5; break;
        case 'p': return	4; break;
        case 'P': return	8; break;
        case 's': return 	1; break;
        case 'S': return 	1; break;
        case 'q': return	1; break;
        case 'Q': return 	5; break;
        case 'd': return 	9; break;
        case 'D': return   13; break;
        case 'v': return	1; break;
        case 'V': return	5; break;
        default:  return    0; break;
    }
}