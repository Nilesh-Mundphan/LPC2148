#include "system.h"
#include "i2c.h"
#include "oled.h"
#include "fonts.h"
#include "Nstring.h"

#define bitRead(value, bit) (((value) >> (bit)) & 0x01)
#define pgm_read_byte(x) (*(x))

const char *myFontData = ArialMT_Plain_24;

uint8_t display_buffer[1024];
uint8_t lastChar;

void oled_cmd(uint8_t com)
{
	i2c_start();
	i2c_write(OLED_ADDR,0x18);
	i2c_write(0x00,0x28);
	i2c_write(com,0x28);
	i2c_stop();
}

void oled_data(uint8_t data)
{
	i2c_start();
	i2c_write(OLED_ADDR,0x18);
	i2c_write(0x40,0x28);
	i2c_write(data,0x28);
	i2c_stop();
}

void oled_init(void)
{
	i2c_init();
	oled_cmd(0xae);							//display off
	oled_cmd(0xa6);            	//Set Normal Display (default)
															// Adafruit Init sequence for 128x64 OLED module
	oled_cmd(0xAE);             //oled_off
	oled_cmd(0xD5);            	//SETDISPLAYCLOCKDIV
	oled_cmd(0x80);            	// the suggested ratio 0x80
	oled_cmd(0xA8);            	//SSD1306_SETMULTIPLEX
	oled_cmd(0x3F);
	oled_cmd(0xD3);            	//SEToled_offSET
	oled_cmd(0x0);             	//no offset
	oled_cmd(0x40 | 0x0);      	//SETSTARTLINE
	oled_cmd(0x8D);            	//CHARGEPUMP
	oled_cmd(0x14);
	oled_cmd(0x20);             //MEMORYMODE
	oled_cmd(0x00);             //0x0 act like ks0108
	//oled_cmd(0xA0 | 0x1);     //SEGREMAP   //Rotate screen 180 deg
	oled_cmd(0xA0);
	//oled_cmd(0xC8);           //COMSCANDEC  Rotate screen 180 Deg
	oled_cmd(0xC0);
	oled_cmd(0xDA);            	//0xDA
	oled_cmd(0x12);           	//COMSCANDEC
	oled_cmd(0x81);           	//SETCONTRAS
	oled_cmd(0xCF);           	//
	oled_cmd(0xd9);          		//SETPRECHARGE
	oled_cmd(0xF1);
	oled_cmd(0xDB);        			//SETVCOMDETECT
	oled_cmd(0x40);
	oled_cmd(0xA4);        			//DISPLAYALLON_RESUME
	oled_cmd(0xA6);        			//NORMALDISPLAY
	//clear_display();
	oled_cmd(0x2e);            	// stop scroll
	//----------------------------REVERSE comments----------------------------//
	oled_cmd(0xa0);    					//seg re-map 0->127(default)
	oled_cmd(0xa1);    					//seg re-map 127->0
	oled_cmd(0xc8);
	delay_ms(1000);
	//----------------------------REVERSE comments----------------------------//
	// oled_cmd(0xa7);  				//Set Inverse Display
	// oled_cmd(0xae);  				//display off
	oled_cmd(0x20);            	//Set Memory Addressing Mode
	oled_cmd(0x00);            	//Set Memory Addressing Mode ab Horizontal addressing mode
	//  oled_cmd(0x02);         // Set Memory Addressing Mode ab Page addressing mode(RESET)
}

void oled_reset(void)
{
	oled_off();
	oled_clear();
	oled_on();
}

void oled_on(void)
{
	oled_cmd(0xaf);        //display on
}

void oled_off(void)
{
	oled_cmd(0xae);    //display off
}

void oled_fill(uint8_t fill_data)
{
	uint16_t i;
	for (i=0;i<1024;i++)
	{
		display_buffer[i]=fill_data;
	}
	oled_update();
}

void oled_update(void)
{
	uint16_t i;
	uint8_t x;
	oled_cmd(0x21);
	oled_cmd(0x0);
	oled_cmd(0x7F);
	oled_cmd(0x22);
	oled_cmd(0x0);
	oled_cmd(0x7);
	for (i = 0; i<(128 * 64 / 8); i++) 
	{
		i2c_start();
		i2c_write(OLED_ADDR,0x18);
		i2c_write(0x40,0x28);
		for (x = 0; x<16; x++) {
			i2c_write(display_buffer[i],0x28);
			i++;
		}
		i--;
		i2c_stop();
	}
}

void oled_clear(void)
{
	oled_fill(0x00);
}

void oled_set_pixel(uint8_t x, uint8_t y,uint8_t myColor) 
{
	if (x && x < 128 && y && y < 64) 
	{
		switch (myColor) 
		{
			case WHITE:   display_buffer[x + (y/8)*128] |=  (1 << (y&7)); break;
			case BLACK:   display_buffer[x + (y/8)*128] &= ~(1 << (y&7)); break;
			case INVERSE: display_buffer[x + (y/8)*128] ^=  (1 << (y&7)); break;
		}
	}
}

void oled_print_xy(int x, int y, char * text) {
	int j,m,i,bit;
	unsigned char currentByte;
	int charX, charY;
	int currentBitCount;
	int charCode;
	int currentCharWidth;
	int currentCharStartPos;
	int cursorX = 0;
	int numberOfChars = pgm_read_byte(myFontData + CHAR_NUM_POS);
	int charHeight = pgm_read_byte(myFontData + HEIGHT_POS);
	int currentCharByteNum = 0;
	int startX = x;
	int startY = y;

	for (j=0; j < str_len(text); j++) 
	{
		charCode = text[j]-0x20;

		currentCharWidth = pgm_read_byte(myFontData + CHAR_WIDTH_START_POS + charCode);
		currentCharStartPos = CHAR_WIDTH_START_POS + numberOfChars;

		for (m = 0; m < charCode; m++) 
		{
			currentCharStartPos += pgm_read_byte(myFontData + CHAR_WIDTH_START_POS + m)  * charHeight / 8 + 1;
		}

		currentCharByteNum = ((charHeight * currentCharWidth) / 8) + 1;
		for (i = 0; i < currentCharByteNum; i++) 
		{
			currentByte = pgm_read_byte(myFontData + currentCharStartPos + i);
			for( bit = 0; bit < 8; bit++) 
			{
				currentBitCount = i * 8 + bit;
				charX = currentBitCount % currentCharWidth;
				charY = currentBitCount / currentCharWidth;

				if (bitRead(currentByte, bit)) 
				{
					oled_set_pixel(startX + cursorX + charX, startY + charY,WHITE);
				}
			}
		}
		cursorX += currentCharWidth;
	}
	oled_update();
}
