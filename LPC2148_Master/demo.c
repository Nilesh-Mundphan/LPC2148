#include "delay.h"
#include "gpio.h"
#include "lcd.h"
#include "serial.h"
#include "adc.h"
#include "gsm.h"
#include "hcsr04.h"
#include "i2c.h"
#include "Nstring.h"
#include <stdio.h>

#include "demo.h"
#include "spi.h"
#include "mcp320x.h"

void mcp32_test(void)
{
    char val[32];
    uint16_t result;
	  spi_init();
    serial0_init(9600);
    serial0_print("External SPI Test\r\n");
    
		while(1)
		{
				result=read_mcp320x(0);
				sprintf(val,"Channel 0 :%5.0d",result);
				serial0_print(val);
        serial0_print("\r\n");
				delay_ms(1000);
	  }
}

void set_time_date(uint8_t *rtc_str)
{
	i2c_write_buffer(0,rtc_str,7);
}
void hex_to_str(uint8_t value,char *str)
{
	str[0]=48+((value & 0x70)>>4);
	str[1]=48+(value & 0x0f);
}

void get_time_date(char *rtc_str)
{
	uint8_t rtc_buffer[8];
	char h_s[2];
	if (!i2c_read_buffer(0,rtc_buffer,7))		// Read date and time from RTC 
			serial0_print("\nMemory Read error....");
  	
	
	hex_to_str(rtc_buffer[4],h_s);
	rtc_str[0]=h_s[0];rtc_str[1]=h_s[1];rtc_str[2]='/';
	hex_to_str(rtc_buffer[5],h_s);
	rtc_str[3]=h_s[0];rtc_str[4]=h_s[1];rtc_str[5]='/';
	hex_to_str(rtc_buffer[6],h_s);
	rtc_str[6]=h_s[0];rtc_str[7]=h_s[1];rtc_str[8]=' ';
	
	hex_to_str(rtc_buffer[2],h_s);
	rtc_str[9]=h_s[0];rtc_str[10]=h_s[1];rtc_str[11]=':';
	hex_to_str(rtc_buffer[1],h_s);
	rtc_str[12]=h_s[0];rtc_str[13]=h_s[1];rtc_str[14]=':';
	hex_to_str(rtc_buffer[0],h_s);
	rtc_str[15]=h_s[0];rtc_str[16]=h_s[1];rtc_str[17]=' ';
	
	if(rtc_buffer[2] > 0x12)
	{
		rtc_str[18]='P';
		rtc_str[19]='M';
	}
	else
	{
		rtc_str[18]='A';
		rtc_str[19]='M';
	}
	rtc_str[20]='\0';
}

void i2c_rtc_test(void)
{	
	serial0_init(9600);
	serial0_print("I2C RTC Testing\r\n");
	i2c_init();
	while(1)
	{
		uint8_t read_buffer[24];
		get_time_date((char *)read_buffer);
		serial0_print((char *)read_buffer);	
		serial0_print("\r\n");	
		delay_ms(1000);
	}

}

void i2c_eeprom_test(void)
{
	uint8_t write_buffer[8]="Nilesh",read_buffer[8];
  	
	serial0_init(9600);
	serial0_print("I2C Testing\r\n");
	i2c_init();
	i2c_write_buffer(0,write_buffer,6);
	serial0_print("Write Done\r\n");
	
	while(1)
	{
		if (!i2c_read_buffer(0,read_buffer,6))		// Read date and time from RTC 
			serial0_print("\nMemory Read error....");
		
		serial0_print((char *)read_buffer);	
		serial0_print("\r\n");	
		delay_ms(1000);
	}
}
void hcsr04_test(void)
{
    uint32_t d=0;
    char line1[16];    
		
		serial0_init(9600);
	  serial0_print("HCSR04 Testing\r\n");
		HCSR04_init();
	  while(1)
    {
        HCSR04_trigger();
        d=get_pulse_width();
        
        sprintf(line1,"Distance : %d",(d/58));
        serial0_print(line1);
			  serial0_print("\r\n");
				delay_ms(500);
    }
}

void gsm_test(void)
{
    char msg[30],no[15],id[2];
    char pname[10];
    serial0_init(9600);
	  serial0_print("GSM Testing\r\n");
		
	  gsm_init();
    gsm_pname(pname);
    serial0_print(pname);
		serial0_print("\r\n");
		delay_ms(2000);

    gsm_msg_send("+919960226605","GSM Testing");
    while(1)
		{
		while(gsm_wait(id)==0)
		{
			serial0_print("Waiting For Msg\r\n");
			delay_ms(200);
		}
        
        gsm_msg_read(no,msg,id);
				serial0_print(no);
				serial0_print("\r\n");
		    serial0_print(msg);
				serial0_print("\r\n");
		
    		delay_ms(5000);
        
	}
}


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
