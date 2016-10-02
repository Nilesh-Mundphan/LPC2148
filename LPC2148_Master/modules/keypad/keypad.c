#include "system.h"
#include "keypad.h"

uint8_t keypad_pins[8]={32,33,34,35,38,39,40,41};

uint8_t get_keypad_key(void)
{
uint8_t r,c,i;

	for(i=0;i<ROWS;i++)
		gpio_pin_mode(keypad_pins[i],INPUT);
	
	for(i=0;i<COLUMNS;i++)
		gpio_pin_mode(keypad_pins[i+4],OUTPUT);

	for(c=0;c<COLUMNS;c++)
	{
		for(i=0;i<( COLUMNS+ROWS );i++)
			gpio_pin_write(keypad_pins[i],HIGH);

		gpio_pin_write(keypad_pins[COLUMNS + ROWS - 1 - c],LOW);

		for(r=0;r<ROWS;r++)
		{
			if(!(gpio_pin_read(keypad_pins[ROWS - 1 - r])))
			{
				return (r*ROWS+c);
			}
		}
	}
return 0XFF;//Indicate No key pressed
}
