#include <LPC214X.H>
#include <stdio.h>
#include "demo.h"
#include "serial.h"
#include "adc.h"
#include "delay.h"
#include "gsm.h"
#include "dht11.h"

void dht11_test()
{
    char disp[20];
    serial0_init(9600);
    serial0_print("DHT11 Testing ....\r\n");
    while(1)
    {
        dht_read11(DHT_PIN);
        serial0_print("--------------------\r\n");
				sprintf(disp,"Temperature :%d",temperature);
				serial0_print(disp);
        serial0_print("\r\n");
        sprintf(disp,"Humidity :%d",humidity);
				serial0_print(disp);
        serial0_print("\r\n");
        serial0_print("--------------------\r\n");
        
        delay_ms(1000);
    }
}

int main(void)
{
    dht11_test();
}

