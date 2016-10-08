#ifndef MPU6050_H_
#define	MPU6050_H_

#define MUP_ADDR 0xD0   
    
void mpu6050_init(void);
void mpu6050_read(int16_t*);

#endif
