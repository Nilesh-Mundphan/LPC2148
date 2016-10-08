#ifndef NRF24_H_
#define NRF24_H_

#define NRF_CE    7
#define NRF_CSN   8

#define bitRead(value, bit) (((value) >> (bit)) & 0x01)
#define bitSet(value, bit) ((value) |= (1UL << (bit)))
#define bitClear(value, bit) ((value) &= ~(1UL << (bit)))
#define bitWrite(value, bit, bitvalue) (bitvalue ? bitSet(value, bit) : bitClear(value, bit))

void nrf_init(void);
void nrf_write_bit(byte address, byte bit_add, byte val);
void nrf_send_byte(uint8_t *bytes,uint8_t no_bits);
void nrf_irq(void) __irq;
void nrf_clear_interrupt(void);
void nrf_get_address(byte address, byte info);
void nrf_set_rx_payload(byte pipe, byte bytes);
void nrf_data_integrity(void);

#endif 
