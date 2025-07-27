#include "ultrasonic.h"
#include "timer_delay.h"

static TIM_HandleTypeDef *ultra_htim;

static volatile uint32_t ic_val1 = 0;
static volatile uint32_t ic_val2 = 0;
static volatile uint32_t echo_time = 0;
static volatile uint8_t is_first_cap = 0;
static volatile uint8_t distance = 0;

#define MIN_ECHO_TIME 116 // Tương ứng 2 cm
#define MAX_ECHO_TIME 23333 // Tương ứng 400 cm

void ultrasonic_init(TIM_HandleTypeDef *htim) {
    ultra_htim = htim;
    HAL_TIM_IC_Start_IT(ultra_htim, TIM_CHANNEL_1);
}

void HCSR04_trig(void) {
    HAL_GPIO_WritePin(TRIGGER_GPIO_Port, TRIGGER_Pin, GPIO_PIN_SET);
    delay_us(10);
    HAL_GPIO_WritePin(TRIGGER_GPIO_Port, TRIGGER_Pin, GPIO_PIN_RESET);
}

void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim) {
    if (htim->Channel == HAL_TIM_ACTIVE_CHANNEL_1) {
        if (is_first_cap == 0) {
            ic_val1 = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_1);
            is_first_cap = 1;
            __HAL_TIM_SET_CAPTUREPOLARITY(htim, TIM_CHANNEL_1, TIM_INPUTCHANNELPOLARITY_FALLING);
        } else {
            ic_val2 = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_1);
            __HAL_TIM_SET_COUNTER(htim, 0);

            if (ic_val2 >= ic_val1) {
                echo_time = ic_val2 - ic_val1;
            } else {
                echo_time = 0xFFFF + ic_val2 - ic_val1;
            }

            // Kiểm tra lỗi
            if (echo_time < MIN_ECHO_TIME || echo_time > MAX_ECHO_TIME) {
                distance = 0;
                is_first_cap = 0;
                __HAL_TIM_SET_CAPTUREPOLARITY(htim, TIM_CHANNEL_1, TIM_INPUTCHANNELPOLARITY_RISING);
                return;
            }

            // Tính khoảng cách bằng số nguyên
            uint32_t distance_temp = (echo_time * 343) / 20000; // 343/20000 ≈ 0.01715 cm/µs
            if (distance_temp >= 2 && distance_temp <= 400) {
                distance = (uint8_t)distance_temp;
            } else {
                distance = 0;
            }

            is_first_cap = 0;
            __HAL_TIM_SET_CAPTUREPOLARITY(htim, TIM_CHANNEL_1, TIM_INPUTCHANNELPOLARITY_RISING);
        }
    }
}

uint8_t get_distance(void) {
    uint32_t sum = 0;
    uint8_t valid = 0;
    uint8_t temp_distance;

    for (uint8_t i = 0; i < 5; i++) {
        is_first_cap = 0;
        HCSR04_trig();
        HAL_Delay(1);
        HAL_Delay(10);

        __disable_irq();
        temp_distance = distance;
        __enable_irq();

        if (temp_distance >= 2 && temp_distance <= 400) {
            sum += temp_distance;
            valid++;
        }
    }

    if (valid == 0) return 0;
    return (uint8_t)(sum / valid);
}
