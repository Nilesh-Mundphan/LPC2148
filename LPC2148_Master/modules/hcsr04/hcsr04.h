/* 
 * File:   hcsr04.h
 * Author: Nilesh
 *
 * Created on July 7, 2016, 10:09 PM
 */

#ifndef HCSR04_H
#define	HCSR04_H

#ifdef	__cplusplus
extern "C" {
#endif

#define US_TRIG_POS  8
#define US_ECHO_POS  9

#define US_ERROR     				8888
#define US_NO_OBSTACLE      7777
    
#define US_PRESCALE 15
	
void HCSR04_init(void);
void HCSR04_trigger(void);
uint32_t get_pulse_width(void);

    
    
#ifdef	__cplusplus
}
#endif

#endif	/* HCSR04_H */

