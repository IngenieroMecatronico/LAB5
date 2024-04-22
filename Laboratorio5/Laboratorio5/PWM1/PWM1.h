/*
 * PWM1.h
 *
 * Created: 15/04/2024 01:33:27
 *  Author: ferma
 */ 


#ifndef PWM1_H_
#define PWM1_H_

#include <avr/io.h>
#include <stdint.h>

int PWM1(uint8_t numeropot, uint8_t valoradc, uint8_t overflow);
//int LED(uint8_t pot, uint8_t valoradc, uint8_t overflow);

#endif /* PWM1_H_ */