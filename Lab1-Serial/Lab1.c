#include "../c_lib/SerialIO.h"

/** Main program entry point. This routine configures the hardware required by the application, then
 *  enters a loop to run the application tasks in sequence.
 */
int main(void)
{
    USB_SetupHardware();
    GlobalInterruptEnable();

    for (;;)
    {
        USB_Echo_Task();
        USB_USBTask();
    }
}
