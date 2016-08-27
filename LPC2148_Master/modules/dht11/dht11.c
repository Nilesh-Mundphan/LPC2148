/*
 * dht11.c
 *
 * Created: 5/29/2016 5:54:20 PM
 *  Author: Nilesh
 */ 

 #include "system.h"
 #include "dht11.h"
 #include "delay.h"
 
 uint8_t humidity=0;
 uint8_t temperature=0;
 uint8_t bits[5];
 
 int8_t dht_read11(uint8_t pin)
 {
	 uint8_t sum;
	 int8_t result = dht_readSensor(pin, DHTLIB_DHT11_WAKEUP, DHTLIB_DHT11_LEADING_ZEROS);
	 bits[0] &= 0x7F;
	 bits[2] &= 0x7F;
	 // CONVERT AND STORE
	 humidity    = bits[0];  // bits[1] == 0;
	 temperature = bits[2];  // bits[3] == 0;
	 // TEST CHECKSUM
	 // bits[1] && bits[3] both 0
	 sum = bits[0] + bits[2];
	 if (bits[4] != sum)
	 {
		 return DHTLIB_ERROR_CHECKSUM;
	 }
	 return result;
 }

 int8_t dht_readSensor(uint8_t pin, uint8_t wakeupDelay, uint8_t leadingZeroBits)
 {
	 // INIT BUFFERVAR TO RECEIVE DATA
	 uint8_t mask = 128;
	 uint8_t idx = 0;
	 uint8_t data = 0;
	 uint8_t state = LOW;
	 uint8_t pstate = LOW;
	 uint16_t zeroLoop = DHTLIB_TIMEOUT;
	 uint16_t delta = 0;
	 uint16_t loopCount=0;	
	 uint8_t i;
	 
	 leadingZeroBits = 40 - leadingZeroBits; // reverse counting...
	 DHT_OUTPUT();
	 DHT_LOW();
	 delay_ms(DHTLIB_DHT11_WAKEUP);
	 DHT_HIGH();
	 DHT_INPUT();
	 loopCount = DHTLIB_TIMEOUT * 2;  // 200uSec max
	 while ((DHT_READ() & 1) != LOW )
	 {
		 if (--loopCount == 0) return DHTLIB_ERROR_CONNECT;
	 }
	 // GET ACKNOWLEDGE or TIMEOUT
	 loopCount = DHTLIB_TIMEOUT;
	 while ((DHT_READ() & 1) == LOW )  // T-rel
	 {
		 if (--loopCount == 0) return DHTLIB_ERROR_ACK_L;
	 }
	 loopCount = DHTLIB_TIMEOUT;
	 while ((DHT_READ() & 1) != LOW )  // T-reh
	 {
		 if (--loopCount == 0) return DHTLIB_ERROR_ACK_H;
	 }
	 loopCount = DHTLIB_TIMEOUT;
	 // READ THE OUTPUT - 40 BITS => 5 BYTES
	 for ( i = 40; i != 0; )
	 {
		 // WAIT FOR FALLING EDGE
		 state = (DHT_READ() & 1);
		 if (state == LOW && pstate != LOW)
		 {
			 if (i > leadingZeroBits) // DHT22 first 6 bits are all zero !!   DHT11 only 1
			 {
				 if(zeroLoop > loopCount)
				 zeroLoop = loopCount;
				 delta = (DHTLIB_TIMEOUT - zeroLoop)/4;
			 }
			 else if ( loopCount <= (zeroLoop - delta) ) // long -> one
			 {
				 data |= mask;
			 }
			 mask >>= 1;
			 if (mask == 0)   // next byte
			 {
				 mask = 128;
				 bits[idx] = data;
				 idx++;
				 data = 0;
			 }
			 --i;  // next bit
			 loopCount = DHTLIB_TIMEOUT; // reset timeout flag
		 }
		 pstate = state;
		 // Check timeout
		 if (--loopCount == 0)
		 {
			 return DHTLIB_ERROR_TIMEOUT;
		 }
	 }
	 DHT_OUTPUT();
	 DHT_HIGH();
	 return DHTLIB_OK;
 }
