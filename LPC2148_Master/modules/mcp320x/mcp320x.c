#include "system.h"
#include "spi.h"

uint16_t read_mcp320x(uint8_t channel)
{
		uint8_t byte,high=0,low=0;
     
		byte=0x60 | (channel << 2);
    
	  gpio_pin_mode(7,OUTPUT); 
    gpio_pin_write(7,LOW);

					 spi_data(byte);
		high = spi_data(0xFF);
		low  = spi_data(0xFF);

		gpio_pin_write(7,HIGH);

	return ((high << 4 ) | (low >> 4)) & 0xFFF;
}
