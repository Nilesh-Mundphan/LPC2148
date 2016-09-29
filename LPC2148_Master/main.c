#include "system.h"
#include "demo.h"
#include "serial.h"
#include "oled.h"

int main(void)
{
		oled_init();
		oled_reset();
		oled_fill(0x55);
		delay_ms(500);
		oled_clear();
		oled_fill(0x00);
		oled_print_xy(0,0,"Latest");
		oled_print_xy(0,30,"Embedded");
		while(1);
}
