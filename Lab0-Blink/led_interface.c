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

void dot_dash_blink(char* c_str, uint32_t unit_length)
{
    uint8_t ind = 0;
    while( c_str[ind] != 0 && ind < 8) // morse patterns are less than 5 dots and dashes with 7 for long pauses
    {
        if( c_str[ind] == '.' )
            morse_dot(unit_length);
        else
            morse_dash(unit_length);
        ind ++;
    }
    morse_letter_end(unit_length);
}

void blink_morse_char(char value, uint32_t unit_length)
{
    switch( tolower(value) )
    {
        case 'a':
            // .-
            dot_dash_blink(".-", unit_length);
            break;
        case 'b':
            //-...
            dot_dash_blink("-...", unit_length);
            break;
        case 'c':
            //-.-.
            dot_dash_blink("-.-.", unit_length);
            break;
        case 'd':
            //-...
            dot_dash_blink("-...", unit_length);
            break;
        case 'e':
            //.
            dot_dash_blink(".", unit_length);
            break;
        case 'f':
            //..-.
            dot_dash_blink("..-.", unit_length);
            break;
        case 'h':
            //....
            dot_dash_blink("....", unit_length);
            break;
        case 'i':
            //..
            dot_dash_blink("..", unit_length);
            break;
        case 'j':
            //.---
            dot_dash_blink(".---", unit_length);
            break;
        case 'k':
            //-.-
            dot_dash_blink("-.-", unit_length);
            break;
        case 'l':
            //.-..
            dot_dash_blink(".-..", unit_length);
            break;
        case 'm':
            //--
            dot_dash_blink("--", unit_length);
            break;
        case 'n':
            //-.
            dot_dash_blink("-.", unit_length);
            break;
        case 'o':
            //---
            dot_dash_blink("---", unit_length);
            break;
        case 'p':
            //.--.
            dot_dash_blink(".--.", unit_length);
            break;
        case 'q':
            //--.-
            dot_dash_blink("--.-", unit_length);
            break;
        case 'r':
            //.-.
            dot_dash_blink(".-.", unit_length);
            break;
        case 's':
            //...
            dot_dash_blink("...", unit_length);
            break;
        case 't':
            //-
            dot_dash_blink("-", unit_length);
            break;
        case 'u':
            //..-
            dot_dash_blink("..-", unit_length);
            break;
        case 'v':
            //...-
            dot_dash_blink("...-", unit_length);
            break;
        case 'w':
            //.--
            dot_dash_blink(".--", unit_length);
            break;
        case 'x':
            //-..-
            dot_dash_blink("-..-", unit_length);
            break;
        case 'y':
            //-.--
            dot_dash_blink("-.--", unit_length);
            break;
        case 'z':
            //--..
            dot_dash_blink("--..", unit_length);
            break;
        case '1':
            dot_dash_blink(".____", unit_length);
            break;
        case '2':
            dot_dash_blink("..___", unit_length);
            break;
        case '3':
            dot_dash_blink("...__", unit_length);
            break;
        case '4':
            dot_dash_blink("...._", unit_length);
            break;
        case '5':
            dot_dash_blink(".....", unit_length);
            break;
        case '6':
            dot_dash_blink("-....", unit_length);
            break;
        case '7':
            dot_dash_blink("--...", unit_length);
            break;
        case '8':
            dot_dash_blink("---..", unit_length);
            break;
        case '9':
            dot_dash_blink("----.", unit_length);
            break;
        case '0':
            dot_dash_blink("-----", unit_length);
            break;
        case ' ':
            led_off_block(7*unit_length);
            break;
        case '.':
            // .-.-.-
            dot_dash_blink(".-.-.-", unit_length);
            break;
        case '!':
            // -.-.--
            dot_dash_blink("-.-.--", unit_length);
            break;
        case '?':
            //..--..
            dot_dash_blink("..--..", unit_length);
            break;
        default:
            led_off_block(7*unit_length);
            break;
    }
}


