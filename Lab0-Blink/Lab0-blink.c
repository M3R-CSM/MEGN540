#define F_CPU 16000000
#include "led_interface.h"

/**
 * Function blink_string takes a c-string and blinks the morse code corresponding to the characters provided.
 * @param str pointer to a null-germinated c-string object
 * @param dot_length_ms The characteristic length of a dot in the morse-code pattern.
 */
void blink_morse_str(char* str, uint16_t dot_length_ms)
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

  while(1)
  {
      // blink message!
      blink_morse_char('S',dot_length_ms);
      blink_morse_char('O',dot_length_ms);
      blink_morse_char('S',dot_length_ms);
      blink_morse_char(' ',dot_length_ms);

      // Using your blink_morse_str function this would be called as:
      // blink_morse_str("SOS ", dot_length_ms);
  }
}
