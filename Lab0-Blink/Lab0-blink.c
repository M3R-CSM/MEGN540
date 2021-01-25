#define F_CPU 16000000
#include "led_interface.h"

int main()
{
    initialize_LED();
    const uint16_t dot_length_ms = 250;

  while(1)
  {
      // blink message!
      blink_morse_char('S',dot_length_ms);
      blink_morse_char('O',dot_length_ms);
      blink_morse_char('S',dot_length_ms);
      blink_morse_char(' ',dot_length_ms);
  }
}
