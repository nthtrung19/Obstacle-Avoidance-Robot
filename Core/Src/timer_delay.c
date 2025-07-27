#include "timer_delay.h"

static TIM_HandleTypeDef *_htim;

void timer_delay_init(TIM_HandleTypeDef *htim) {
    _htim = htim;
    HAL_TIM_Base_Start(_htim);
}

void delay_us(uint16_t us) {
    __HAL_TIM_SET_COUNTER(_htim, 0);  // Reset bộ đếm
    while (__HAL_TIM_GET_COUNTER(_htim) < us);
}

void delay_ms(uint16_t ms) {
    while (ms--) {
        delay_us(1000);
    }
}
