#include "system.h"
#include "i2c.h"
#include "mpu6050.h"

void mpu6050_init(void)
{
	uint8_t f_val=0;
    i2c_init();   
    i2c_write_buffer(MUP_ADDR,0x6B,&f_val,1); 	
}

void mpu6050_read(int16_t *r_buf)
{
	        uint8_t str[14];
					i2c_read_buffer(MUP_ADDR,0x3B,str,14);
					r_buf[0]=str[0]<<8|str[1];
					r_buf[1]=str[2]<<8|str[3];
					r_buf[2]=str[4]<<8|str[5];
	
					r_buf[3]=str[6]<<8|str[7];
	
					r_buf[4]=str[8]<<8|str[9];
					r_buf[5]=str[10]<<8|str[11];
					r_buf[6]=str[12]<<8|str[13];
}
