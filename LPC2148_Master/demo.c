#include "delay.h"
#include "gpio.h"
#include "lcd.h"
#include "serial.h"
#include "adc.h"
#include <stdio.h>

void adc_test()
{
char ds[128];
uint16_t adc_data1,adc_data2,adc_data3,adc_data6,adc_data7;
adc0_init();
serial0_init(9600);	

while(1)
	{
		adc_data1=adc0_read(1);
		adc_data2=adc0_read(2);
		adc_data3=adc0_read(3);
		
		adc_data6=adc0_read(6);
		adc_data7=adc0_read(7);
		
		sprintf(ds,"ADC1: %5.0d ADC2: %5.0d ADC3: %5.0d ADC6: %5.0d ADC7: %5.0d",adc_data1,adc_data2,adc_data3,adc_data6,adc_data7);
	  serial0_print(ds);
		serial0_print("\r\n");
		delay_ms(1000);
	}
}

void serial_test()
{
		serial0_init(9600);
	  serial0_print("Hello LPC\r\n");
	  while(1){
	  if(serial0_available()>0)
		{
			serial0_write(serial0_read());
	  }
	}
}

void gpio_test(void)
{
    gpio_pin_mode(32,OUTPUT);
	  while(1)
    {
      gpio_pin_write(32,LOW);
    	delay_ms(1000);
    	gpio_pin_write(32,HIGH);
      delay_ms(1000);
    }
}

void lcd_test(void)
{
		lcd_init(0,1,2,3,32,33);
    lcd_clear();
    lcd_print_xy(0,0,"#   LCP2148   #");
    lcd_print_xy(0,1,"###  Board  ###");

}