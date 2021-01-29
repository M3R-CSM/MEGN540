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
#include <avr/io.h>
#include <avr/wdt.h>
#include <avr/power.h>
#include <avr/interrupt.h>
#include <string.h>

#include "USB_Config/Descriptors.h"
#include <LUFA/Drivers/USB/USB.h>
#include <LUFA/Platform/Platform.h>

// *** MEGN540  ***
// Include your Ring_Buffer homework code.
#include "Ring_Buffer.h"


/* LUFA Specific Function Prototypes: */
void USB_SetupHardware(void);  // You'll need to add in any initialization items to this function for your ring buffers

/**
 * Function USB_Upkeep_Task shoudl be called each loop in the main function.
 */
void USB_Upkeep_Task(void);    // You'll need to add in USB buffer interaction here. This is where calls to usb_read_nex_byte would go...

void USB_Echo_Task(void)
void EVENT_USB_Device_Connect(void);
void EVENT_USB_Device_Disconnect(void);
void EVENT_USB_Device_ConfigurationChanged(void);
void EVENT_USB_Device_ControlRequest(void);


/* MEGN540 Specific Functions */
/**
 * (non-blocking) Function usb_read_next_byte takes the next USB byte and reads it
 * into a ring buffer for latter processing.
 *
 */
void usb_read_next_byte();

/**
 * (non-blocking) Function usb_write_next_byte takes the next byte from the output
 * ringbuffer and writes it to the USB port (if free).
 */
void usb_write_next_byte();

/**
 * (non-blocking) Function usb_send_byte Adds a character to the output buffer
 * @param byte [uint8_t] Data to send
 */
void usb_send_byte(uint8_t byte);

/**
 * (non-blocking) Function usb_send_data adds the data buffer to the output ring buffer.
 * @param p_data [void*] pointer to the data-object to be sent
 * @param data_len [uint8_t] size of data-object to be sent
 */
void usb_send_data(void* p_data, uint8_t data_len);

/**
 * (non-blocking) Function usb_send_str adds a c-style (null terminated) string to the output buffer
 * @param p_str [char*] Pointer to a c-string (null terminated) to send
 */
void usb_send_str(char* p_str);

/**
 * (non-blocking) Function usb_send_msg sends a message according to the MEGN540 USB message format.
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
void usb_send_msg(char* format, char cmd, void* p_data, uint8_t data_len );

/**
 * (non-blocking) Funtion usb_msg_length returns the number of bytes in the receive buffer awaiting processing.
 * @return [uint8_t] Number of bytes ready for processing.
 */
uint8_t usb_msg_length();

/**
 * (non-blocking) Function usb_msg_peek returns (without removal) the next byte in teh receive buffer (null if empty).
 * @return [uint8_t] Next Byte
 */
uint8_t usb_msg_peek();

/**
 * (non-blocking) Function usb_msg_get removes and returns the next byte in the receive buffer (null if empty)
 * @return [uint8_t] Next Byte
 */
uint8_t usb_msg_get();

/**
 * (non-blocking) Function usb_msg_read_into populates the object with the data in the recieve buffer and
 * removes the bytes as they are read.  Returns false if receive buffer does not contain enough bytes to
 * fill the container and terminates without reading or removing.
 *
 * @param buff
 * @param data_len
 * @return [bool]  True: sucess, False: not enough bytes available
 */
bool usb_msg_read_into(void* p_obj, uint8_t data_len);

/**
 * (non-blocking) Function usb_flush_input_buffer sets the length of the recieve buffer to zero and disreguards
 * any bytes that remaining.
 */
void usb_flush_input_buffer();

#endif

