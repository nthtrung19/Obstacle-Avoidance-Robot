#ifndef __TIMER_DELAY_H__
#define __TIMER_DELAY_H__

#include "stm32f4xx_hal.h"

// Khởi tạo timer để tạo delay (us và ms)
void timer_delay_init(TIM_HandleTypeDef *htim);

// Delay chính xác bằng micro giây
void delay_us(uint16_t us);

// Delay chính xác bằng mili giây (khác với HAL_Delay)
void delay_ms(uint16_t ms);

#endif
