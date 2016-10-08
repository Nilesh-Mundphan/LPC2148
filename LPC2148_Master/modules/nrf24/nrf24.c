#include "system.h"
#include "serial.h"
#include "spi.h"
#include "extint.h"
#include "nrf24.h"

#define byte uint8_t
byte data_in[5], data2, data3;
uint8_t nrf_rec_buff[9];

void nrf_send_byte(uint8_t *bytes,uint8_t no_bits)
{
	uint8_t i;
	gpio_pin_write(NRF_CSN, LOW);
	data_in[0] = spi_data(0xE1);  			//flush TX, get rid of anything that might be in there
	gpio_pin_write(NRF_CSN, HIGH);  

	gpio_pin_write(NRF_CSN, LOW);
	data_in[0] = spi_data(0xA0);   		//load TX payload
	for( i=0;i<no_bits;i++)
		spi_data(bytes[i]);      				//action digital Read
	
	spi_data(8);

	gpio_pin_write(NRF_CSN, HIGH);

	gpio_pin_write(NRF_CE, LOW);        				//pull CE pin LOW
	delay_ms(1);//small delay_ms
	nrf_write_bit(0, 0, 0);        			//go into TX mode
	delay_ms(1);//small delay_ms
	gpio_pin_write(NRF_CE, HIGH);
	delay_ms(1);                 						//this is the time CE pin must be HIGH for before going back into RX mode
													//delay_ms(1) seems to work best for this.  any longer or shorter doesn't work as well
	nrf_write_bit(0, 0, 1);        			//go back into RX mode
}

void nrf_irq(void) __irq
{                						// this routine is called when the IRQ pin is pulled LOW by the NRF
	uint8_t i;

	EXTINT=EINT0;

	gpio_pin_write(NRF_CSN, LOW);

	data_in[0] = spi_data(0x61);     		//read the payload
	for(i=0;i<9;i++)
		nrf_rec_buff[i]= spi_data(0x00);

	gpio_pin_write(NRF_CSN, HIGH);

  serial0_print("Recived Data :"); 
	for(i=0;i<8;i++)
	{  
		serial0_write((char)nrf_rec_buff[i]);
	}
	serial0_print("\r\n");
 	
	gpio_pin_write(NRF_CSN, LOW);
	data_in[0] = spi_data(0xE2);				//flush RX
	gpio_pin_write(NRF_CSN, HIGH);

	nrf_write_bit(7,6,1);						//clear the RX interrupt flag

  VICVectAddr = 0x00;
}
void nrf_init(void)
{
  gpio_pin_mode(NRF_CE, OUTPUT);              //chip enable set as output
  gpio_pin_mode(NRF_CSN, OUTPUT);             //chip select pin as output
  
	//gpio_pin_mode(NRF_MOSI, OUTPUT);            //SPI data out
  //gpio_pin_mode(NRF_MISO, INPUT);             //SPI data in
  //gpio_pin_mode(NRF_SCK, OUTPUT);             //SPI clock out

  serial0_print("NRF Pins Initialized\r\n");

  //SPI.setBitOrder(MSBFIRST);                //SPI Most Significant Bit First
  //SPI.setDataMode(SPI_MODE0);               // Mode 0 Rising edge of data, keep clock low
  //SPI.setClockDivider(SPI_CLOCK_DIV2);      //Run the data in at 16MHz/2 - 8MHz

  gpio_pin_write(NRF_CE, HIGH);              //RX mode
  gpio_pin_write(NRF_CSN, HIGH);             //SPI idle
  //SPI.begin();                            //start up the SPI library
  spi_init(); 
  serial0_print("NRF Ready\r\n");
	
	nrf_set_rx_payload(0, 9);         			//pipe 0-5, bytes 1-32
	nrf_get_address(7, 1);            			//Get Reg7 Status, 1=print screen
	nrf_write_bit(0, 0, 1);        		//register#, bit#, and value 0 or 1, ::  0,0,1 RX Mode
	nrf_write_bit(0, 1, 1);        		//register, bit, and value 0,1,1 PowerUP
	nrf_write_bit(0, 4, 1);        		//RT Mask turns off the RT interrupt
	nrf_write_bit(0, 5, 1);        		//TX Mask turns off the TX interrupt

	gpio_pin_write(NRF_CSN, LOW);
	data_in[0] = spi_data(0xE2);   	//flush RX
	gpio_pin_write(NRF_CSN, HIGH);
	gpio_pin_write(NRF_CSN, LOW);
	data_in[0] = spi_data(0xE1);   	//flush TX
	gpio_pin_write(NRF_CSN, HIGH);

	nrf_clear_interrupt();            			//clears any interrupts
	delay_ms(100);
//	attachInterrupt(1, nrf_irq, FALLING);    	//kick things off by attachin the IRQ interrupt
	EINT_init(0,EDGE,FALLING_EDGE,(void *)nrf_irq);
}

void nrf_data_integrity(void){
	nrf_send_byte((uint8_t*)"ARM Data",8);   	
}



void nrf_set_rx_payload(byte pipe, byte bytes){   
	byte address=pipe+32+16+1;            			// a register write starts at 32, so add on the 1 and 16 to get you to at R17
	gpio_pin_write(NRF_CSN, LOW);
	data_in[0] = spi_data(address);       		//write register 11 RX_PW_P0
	data_in[1] = spi_data(bytes);         		//3 bytes for now
	gpio_pin_write(NRF_CSN, HIGH);
}

void nrf_write_bit(byte address, byte bit_add, byte val){
													//This routine writes single bits of a register, without affecting the rest of the register
	nrf_get_address(address, 0);            		//first read out the register
	if(val==1)                    					//if we want to write a one to the bit then set the bit in the register we read
		bitSet(data_in[1],bit_add);
	else
		bitClear(data_in[1],bit_add);           		//clear it if not

	gpio_pin_write(NRF_CSN, LOW);           			//now we'll write the modified data back in
	data_in[0] = spi_data(32+address);      	//a write to a register adds 32
	data_in[1] = spi_data(data_in[1]);      	//write the modified register
	gpio_pin_write(NRF_CSN, HIGH);
}

void nrf_clear_interrupt(void){            			  
	//there are three interrupt flags in the NRF.  Thsi routine checks them, and if set, it will clear them

	nrf_get_address(7, 0);              			//RT interrupt
	if(bitRead(data_in[1], 4))
		nrf_write_bit(7,4,1);

	nrf_get_address(7, 0);							//TX interrupt
	if(bitRead(data_in[1], 5))
		nrf_write_bit(7,5,1);

	nrf_get_address(7, 0);							//RX interrupt
	if(bitRead(data_in[1], 6))
		nrf_write_bit(7,6,1);
}

void nrf_get_address(byte address, byte info){  	  
													//send the address and either a 1 or 0 if you want to do a serial print of the address
													//after a call to this routine, data_in[1] will equal the address you called
	gpio_pin_write(NRF_CSN, LOW);
	data_in[0] = spi_data(address);
	data_in[1] = spi_data(0x00);
	gpio_pin_write(NRF_CSN, HIGH);
	if(info==1){									// if the user wanted it, you will get a print out of the register - good fo debugging
		serial0_print("Not yet implemented\r\n");
	}//if 1
}
