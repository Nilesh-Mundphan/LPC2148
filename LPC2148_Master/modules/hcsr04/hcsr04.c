#include "system.h"
#include "hcsr04.h"
#include "gpio.h"
#include "delay.h"

void hcsr04_init(void)
{
    gpio_pin_mode(US_TRIG_POS,OUTPUT);
    gpio_pin_mode(US_ECHO_POS,INPUT);
}

void hcsr04_trigger(void)
{
    gpio_pin_write(US_TRIG_POS,HIGH);
    delay_us(20);
    gpio_pin_write(US_TRIG_POS,LOW);
}

uint32_t get_pulse_width(void)
{
	uint32_t result=0,i=0;
	
	 for(i=0;i<1200000;i++)
   {
      if(!(gpio_pin_read(US_ECHO_POS)))
                    continue; //Line is still low, so wait
      else
                    break; //High edge detected, so break.
   }
	
	T1CTCR = 0x0;
	T1PR = US_PRESCALE-1;
	T1TCR = 0x02;
	T1TCR = 0x01; //Enable timer
  
	for(i=0;i<1200000;i++)
   {
      if((gpio_pin_read(US_ECHO_POS)))
      {
         continue;
      }
      else
          break;
   }
   
	result=T1TC;
	T1TCR = 0x00;
	
 return (result);
}
