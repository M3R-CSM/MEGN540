#include "SerialIO.h"

/** Main program entry point. This routine configures the hardware required by the application, then
 *  enters a loop to run the application tasks in sequence.
 */
int main(void)
{
    USB_SetupHardware();
    GlobalInterruptEnable();
    
      DDRC |= (1 << DDC7);    // Make pin 13 be an output.  


    for (;;)
    {
        PORTC |= (1 << PORTC7);   // Turn the LED on.
        _delay_ms(50);
        PORTC &= ~(1 << PORTC7);  // Turn the LED off.
        _delay_ms(75);
    
        USB_Echo_Task();
        USB_USBTask();
    }
}
