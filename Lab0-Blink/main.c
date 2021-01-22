#define F_CPU 16000000
#include <avr/io.h>
#include <util/delay.h>
 
int main()
{
  DDRC |= (1 << DDC7);    // Make pin 13 be an output.  
  while(1)
  {
    PORTC |= (1 << PORTC7);   // Turn the LED on.
    _delay_ms(100);
    PORTC &= ~(1 << PORTC7);  // Turn the LED off.
    _delay_ms(900);
  }
}
