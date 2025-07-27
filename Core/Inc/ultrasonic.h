#ifndef ULTRASONIC_H
#define ULTRASONIC_H

#include "main.h"

void ultrasonic_init(TIM_HandleTypeDef *htim);
void HCSR04_trig(void);
void delay_us(uint16_t time);
void delay_ms(uint16_t ms);
void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim);
uint8_t get_distance(void);

#endif
