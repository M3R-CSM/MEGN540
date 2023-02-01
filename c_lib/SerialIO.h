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
  Copyright 2020  Dean Camera (dean [at] fourwalledcubicle [dot] com)

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

#ifndef _SERIAL_IO_H_
#define _SERIAL_IO_H_

/* Includes: */
#include "USB_Config/Descriptors.h"
#include <LUFA/Drivers/USB/USB.h>
#include <LUFA/Platform/Platform.h>
#include <avr/interrupt.h>
#include <avr/io.h>
#include <avr/power.h>
#include <avr/wdt.h>

#include <string.h>

// *** MEGN540  ***
// Include your Ring_Buffer homework code.
#ifndef RING_BUFFER_H  // check guard to see if "Ring_Buffer.h" has already been included. If not set ring buffer length
// Define max rignbuffer size here so we have more than enough for the longest message we will send.
#    define RB_LENGTH_B 32
#elif RB_LENGTH_B < 32  // check to see if set RB_LENGTH_IS large enough
#    warning Ring_Buffer created before SerialIO.h. Ring buffer length is too small for serial messages. Consider revising include ordering.
#endif

#include "Ring_Buffer.h"

/**
 * Function Initialize_USB Initializes the USB hardware leveraging the LUFA library and also
 * initializes the internal ring-buffers for USB data storage. Care should be taken to make sure
 * the ring buffer's can accomidate at least two full-sized (16 byte) messages before overflowing. *
 */
void Initialize_USB( void );  // You'll need to add in any initialization items to this function for your ring buffers

/**
 * Function Task_USB_Upkeep should be called each loop in the main function.
 */
void Task_USB_Upkeep( void );  // You'll need to add in USB buffer interaction here. This is where calls to usb_read_nex_byte would go...

/**
 * Function Task_USB_Echo provides a functioning example to help you build your code. This should be removed
 * in the final implementaion.
 */
void Task_USB_Echo( void );

/**
 * (non-blocking) Function USB_Send_Byte Adds a character to the output buffer
 * @param byte [uint8_t] Data to send
 */
void USB_Send_Byte( uint8_t byte );

/**
 * (non-blocking) Function USB_Send_Data adds the data buffer to the output ring buffer.
 * @param p_data [void*] pointer to the data-object to be sent
 * @param data_len [uint8_t] size of data-object to be sent
 */
void USB_Send_Data( void* p_data, uint8_t data_len );

/**
 * (non-blocking) Function USB_Send_Str adds a c-style (null terminated) string to the output buffer
 * @param p_str [char*] Pointer to a c-string (null terminated) to send
 */
void USB_Send_Str( char* p_str );

/**
 * (non-blocking) Function USB_Send_Msg sends a message according to the MEGN540 USB message format.
 *      [MSG Length] [Format C-Str][Host Initiating CMD Char][DATA]
 *      MSG Length: [uint8_t] Number of bytes to follow in full message.
 *              Length of: Format C-String + 1 (CMD Char) + Length of DATA Array
 *      Format C-Str: [char*] null-terminated c-string defining the interpertation of the data bytes.
 *              Valid characters are defined in the Python3 struct defintion.
 *              Some examples are: c (char), b (int8_t), B (uint8_t), h (int16_t), f (float32)
 *      Host Initiating CMD Char: [char] The command sent from the host this message is in response to.
 *           All transmissions are host initiate.
 *      DATA: [Byte Array] Data byes that make up the message to be sent.
 *
 * @param format [c-str pointer] Pointer to interpertation string. e.g. ccf.  This alwasy starts with c because of the
 *          CMD char, here teh DATA object is then a char and a float.
 * @param cmd [char] Command this message is in respose to.
 * @param p_data [void*] pointer to the data-object to send.
 * @param data_len [uint8_t] size of the data-object to send. Remember sizeof() can help you with this!
 */
void USB_Send_Msg( char* format, char cmd, void* p_data, uint8_t data_len );

/**
 * (non-blocking) Funtion USB_Msg_Length returns the number of bytes in the receive buffer awaiting processing.
 * @return [uint8_t] Number of bytes ready for processing.
 */
uint8_t USB_Msg_Length();

/**
 * (non-blocking) Function USB_Msg_Peek returns (without removal) the next byte in teh receive buffer (null if empty).
 * @return [uint8_t] Next Byte
 */
uint8_t USB_Msg_Peek();

/**
 * (non-blocking) Function USB_Msg_Get removes and returns the next byte in the receive buffer (null if empty)
 * @return [uint8_t] Next Byte
 */
uint8_t USB_Msg_Get();

/**
 * (non-blocking) Function USB_Msg_Read_Into populates the object with the data in the recieve buffer and
 * removes the bytes as they are read.  Returns false if receive buffer does not contain enough bytes to
 * fill the container and terminates without reading or removing.
 *
 * @param buff
 * @param data_len
 * @return [bool]  True: sucess, False: not enough bytes available
 */
bool USB_Msg_Read_Into( void* p_obj, uint8_t data_len );

/**
 * (non-blocking) Function USB_Flush_Input_Buffer sets the length of the recieve buffer to zero and disreguards
 * any bytes that remaining.
 */
void USB_Flush_Input_Buffer();

// LUFA Event handeling functions that are defined but do not require modificaiton
void EVENT_USB_Device_Connect( void );
void EVENT_USB_Device_Disconnect( void );
void EVENT_USB_Device_ConfigurationChanged( void );
void EVENT_USB_Device_ControlRequest( void );

#endif
