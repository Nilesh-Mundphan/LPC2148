/*
 * dht11.h
 *
 * Created: 5/29/2016 5:54:33 PM
 *  Author: Nilesh
 */ 


#ifndef DHT11_H_
#define DHT11_H_

#include <stdint.h>
#include "gpio.h"

#define DHT_PIN 5

#define DHT_DDR TRISB
#define DHT_OUT LATB
#define DHT_IN PORTB

//#define DHT_OUTPUT() DHT_DDR&=~(1<<DHT_PIN)
//#define DHT_INPUT() DHT_DDR|=(1<<DHT_PIN)

//#define DHT_HIGH() DHT_IN=DHT_OUT|(1<<DHT_PIN)
//#define DHT_LOW() DHT_IN=DHT_OUT&~(1<<DHT_PIN)

//#define DHT_READ() (DHT_IN>>DHT_PIN)

#define DHT_OUTPUT() gpio_pin_mode(DHT_PIN,OUTPUT)
#define DHT_INPUT() gpio_pin_mode(DHT_PIN,INPUT)

#define DHT_HIGH() gpio_pin_write(DHT_PIN,HIGH)
#define DHT_LOW() gpio_pin_write(DHT_PIN,LOW)

#define DHT_READ() gpio_pin_read(DHT_PIN)



#define DHTLIB_OK                   0
#define DHTLIB_ERROR_CHECKSUM       -1
#define DHTLIB_ERROR_TIMEOUT        -2
#define DHTLIB_ERROR_CONNECT        -3
#define DHTLIB_ERROR_ACK_L          -4
#define DHTLIB_ERROR_ACK_H          -5

#define DHTLIB_DHT11_WAKEUP         18
#define DHTLIB_DHT_WAKEUP           1

#define DHTLIB_DHT11_LEADING_ZEROS  1
#define DHTLIB_DHT_LEADING_ZEROS    6

#define DHTLIB_TIMEOUT 400 // (F_CPU/40000)

extern uint8_t humidity;
extern uint8_t temperature;

extern uint8_t bits[5];  // buffer to receive data
int8_t dht_readSensor(uint8_t pin, uint8_t wakeupDelay, uint8_t leadingZeroBits);
int8_t dht_read11(uint8_t pin);

#endif /* DHT11_H_ */
