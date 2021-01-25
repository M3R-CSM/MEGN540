//
// Created by apetruska on 1/25/21.
//
#include "led_interface.h"

void led_on_block( uint32_t ms )
{
    LED_ON();
    for(uint32_t i=0; i<ms; i++){
        _delay_ms(1);
    }
}
void led_off_block( uint32_t ms)
{
        LED_OFF();
    for(uint32_t i=0; i<ms; i++){
        _delay_ms(1);
    }
}

void morse_dot(uint32_t unit_length)
{
    led_on_block(unit_length);
    led_off_block(unit_length);
}
void morse_dash(uint32_t unit_length)
{
    led_on_block(3*unit_length);
    led_off_block(unit_length);
}
void morse_letter_end(uint32_t unit_length)
{
    led_off_block(2*unit_length);
}

void blink_morse_char(char value, uint32_t unit_length)
{
    switch( tolower(value) )
    {
        case 'a':
            // .-
            morse_dot(unit_length);
            morse_dash(unit_length);
            morse_letter_end(unit_length);
            break;
        case 'b':
            //-...
            morse_dash(unit_length);
            morse_dot(unit_length);
            morse_dot(unit_length);
            morse_dot(unit_length);
            morse_letter_end(unit_length);
            break;
        case 'c':
            //-.-.
            morse_dash(unit_length);
            morse_dot(unit_length);
            morse_dash(unit_length);
            morse_dot(unit_length);
            morse_letter_end(unit_length);
            break;
        case 'd':
            //-...
            morse_dash(unit_length);
            morse_dot(unit_length);
            morse_dot(unit_length);
            morse_letter_end(unit_length);
            break;
        case 'e':
            //.
            morse_dot(unit_length);
            morse_letter_end(unit_length);
            break;
        case 'f':
            //..-.
            morse_dot(unit_length);
            morse_dot(unit_length);
            morse_dash(unit_length);
            morse_dot(unit_length);
            morse_letter_end(unit_length);
            break;
        case 'h':
            //....
            morse_dot(unit_length);
            morse_dot(unit_length);
            morse_dot(unit_length);
            morse_dot(unit_length);
            morse_letter_end(unit_length);
            break;
        case 'i':
            //..
            morse_dot(unit_length);
            morse_dot(unit_length);
            morse_letter_end(unit_length);
            break;
        case 'j':
            //.---
            morse_dot(unit_length);
            morse_dash(unit_length);
            morse_dash(unit_length);
            morse_dash(unit_length);
            break;
        case 'k':
            //-.-
            morse_dash(unit_length);
            morse_dot(unit_length);
            morse_dash(unit_length);
            morse_letter_end(unit_length);
            break;
        case 'l':
            //.-..
            morse_dot(unit_length);
            morse_dash(unit_length);
            morse_dot(unit_length);
            morse_dot(unit_length);
            morse_letter_end(unit_length);
            break;
        case 'm':
            //--
            morse_dash(unit_length);
            morse_dash(unit_length);
            morse_letter_end(unit_length);
            break;
        case 'n':
            //-.
            morse_dash(unit_length);
            morse_dot(unit_length);
            morse_letter_end(unit_length);
            break;
        case 'o':
            //---
            morse_dash(unit_length);
            morse_dash(unit_length);
            morse_dash(unit_length);
            morse_letter_end(unit_length);
            break;
        case 'p':
            //.--.
            morse_dot(unit_length);
            morse_dash(unit_length);
            morse_dash(unit_length);
            morse_dot(unit_length);
            morse_letter_end(unit_length);
            break;
        case 'q':
            //--.-
            morse_dash(unit_length);
            morse_dash(unit_length);
            morse_dot(unit_length);
            morse_dash(unit_length);
            morse_letter_end(unit_length);
            break;
        case 'r':
            //.-.
            morse_dot(unit_length);
            morse_dash(unit_length);
            morse_dot(unit_length);
            morse_letter_end(unit_length);
            break;
        case 's':
            //...
            morse_dot(unit_length);
            morse_dot(unit_length);
            morse_dot(unit_length);
            morse_letter_end(unit_length);
            break;
        case 't':
            //-
            morse_dash(unit_length);
            morse_letter_end(unit_length);
            break;
        case 'u':
            //..-
            morse_dot(unit_length);
            morse_dot(unit_length);
            morse_dash(unit_length);
            morse_letter_end(unit_length);
            break;
        case 'v':
            //...-
            morse_dot(unit_length);
            morse_dot(unit_length);
            morse_dot(unit_length);
            morse_dash(unit_length);
            morse_letter_end(unit_length);
            break;
        case 'w':
            //.--
            morse_dot(unit_length);
            morse_dash(unit_length);
            morse_dash(unit_length);
            morse_letter_end(unit_length);
            break;
        case 'x':
            //-..-
            morse_dash(unit_length);
            morse_dot(unit_length);
            morse_dot(unit_length);
            morse_dash(unit_length);
            morse_letter_end(unit_length);
            break;
        case 'y':
            //-.--
            morse_dash(unit_length);
            morse_dot(unit_length);
            morse_dash(unit_length);
            morse_dash(unit_length);
            morse_letter_end(unit_length);
            break;
        case 'z':
            //--..
            morse_dash(unit_length);
            morse_dash(unit_length);
            morse_dot(unit_length);
            morse_dot(unit_length);
            morse_letter_end(unit_length);
            break;
        case '1':
            morse_dot(unit_length);
            morse_dash(unit_length);
            morse_dash(unit_length);
            morse_dash(unit_length);
            morse_dash(unit_length);
            morse_letter_end(unit_length);
            break;
        case '2':
            morse_dot(unit_length);
            morse_dot(unit_length);
            morse_dash(unit_length);
            morse_dash(unit_length);
            morse_dash(unit_length);
            morse_letter_end(unit_length);
            break;
        case '3':
            morse_dot(unit_length);
            morse_dot(unit_length);
            morse_dot(unit_length);
            morse_dash(unit_length);
            morse_dash(unit_length);
            morse_letter_end(unit_length);
            break;
        case '4':
            morse_dot(unit_length);
            morse_dot(unit_length);
            morse_dot(unit_length);
            morse_dot(unit_length);
            morse_dash(unit_length);
            morse_letter_end(unit_length);
            break;
        case '5':
            morse_dot(unit_length);
            morse_dot(unit_length);
            morse_dot(unit_length);
            morse_dot(unit_length);
            morse_dot(unit_length);
            morse_letter_end(unit_length);
            break;
        case '6':
            morse_dash(unit_length);
            morse_dot(unit_length);
            morse_dot(unit_length);
            morse_dot(unit_length);
            morse_dot(unit_length);
            morse_letter_end(unit_length);
            break;
        case '7':
            morse_dash(unit_length);
            morse_dash(unit_length);
            morse_dot(unit_length);
            morse_dot(unit_length);
            morse_dot(unit_length);
            morse_letter_end(unit_length);
            break;
        case '8':
            morse_dash(unit_length);
            morse_dash(unit_length);
            morse_dash(unit_length);
            morse_dot(unit_length);
            morse_dot(unit_length);
            morse_letter_end(unit_length);
            break;
        case '9':
            morse_dash(unit_length);
            morse_dash(unit_length);
            morse_dash(unit_length);
            morse_dash(unit_length);
            morse_dot(unit_length);
            morse_letter_end(unit_length);
            break;
        case '0':
            morse_dash(unit_length);
            morse_dash(unit_length);
            morse_dash(unit_length);
            morse_dash(unit_length);
            morse_dash(unit_length);
            morse_letter_end(unit_length);
            break;
        case ' ':
            led_off_block(7*unit_length);
            break;
        case '.':
            // .-.-.-
            morse_dot(unit_length);
            morse_dash(unit_length);
            morse_dot(unit_length);
            morse_dash(unit_length);
            morse_dot(unit_length);
            morse_dash(unit_length);
            morse_letter_end(unit_length);
            break;
        case '!':
            // -.-.--
            morse_dash(unit_length);
            morse_dot(unit_length);
            morse_dash(unit_length);
            morse_dot(unit_length);
            morse_dash(unit_length);
            morse_dash(unit_length);
            morse_letter_end(unit_length);
        case '?':
            //..--..
            morse_dot(unit_length);
            morse_dot(unit_length);
            morse_dash(unit_length);
            morse_dash(unit_length);
            morse_dot(unit_length);
            morse_dot(unit_length);
            morse_letter_end(unit_length);
        default:
            led_off_block(7*unit_length);
            break;
    }
}


