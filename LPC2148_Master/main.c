#include <LPC214X.H>
#include <stdio.h>
#include "demo.h"
#include "serial.h"
#include "adc.h"
#include "delay.h"
#include "gsm.h"

void gsm_test()
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

int main(void)
{
    gsm_test();
}

