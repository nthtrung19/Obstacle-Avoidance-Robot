#include "led.h"
#include "timer_delay.h"

// Control LED based on object detection and distance
void led_control(uint8_t object_detected, uint8_t distance) {
    if (object_detected == 0) {
        // No object detected, turn LED off
        HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_RESET);
    } else {
        // Object detected, blink LED with frequency based on distance
        // Linear interpolation: led_delay_ms = 100 + (distance * 400) / 50
        // At 0cm: delay = 100ms (5 Hz), at 50cm: delay = 500ms (1 Hz)
        uint16_t led_delay_ms = 100 + (distance * 400) / 50;

        // Ensure delay is within bounds
        if (led_delay_ms < 100) led_delay_ms = 100;
        if (led_delay_ms > 500) led_delay_ms = 500;

        // Toggle LED
        HAL_GPIO_TogglePin(LED_GPIO_Port, LED_Pin);
        delay_ms(led_delay_ms);
    }
}
