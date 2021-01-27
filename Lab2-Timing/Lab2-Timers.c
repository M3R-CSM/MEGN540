#include "../c_lib/SerialIO.h"
#include "../c_lib/Timing.h"

/** Main program entry point. This routine configures the hardware required by the application, then
 *  enters a loop to run the application tasks in sequence.
 */
int main(void)
{
    SetupTimer0();         // initialize timer zero functionality
    USB_SetupHardware();   // initialize USB

    GlobalInterruptEnable(); // Enable Global Interrupts for USB and Timer etc.

    for (;;)
    {
        USB_Echo_Task();
        USB_USBTask();
    }
}
