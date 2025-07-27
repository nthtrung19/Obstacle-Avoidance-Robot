#ifndef __LED_H
#define __LED_H

#include "main.h"

// LED pin definitions
#define LED_Pin GPIO_PIN_5
#define LED_GPIO_Port GPIOA

// Control LED based on object detection and distance
void led_control(uint8_t object_detected, uint8_t distance);

#endif
