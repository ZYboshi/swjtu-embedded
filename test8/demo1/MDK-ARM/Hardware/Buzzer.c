#include "Buzzer.h"

void PlayBuzzerTone(uint32_t frequency, uint32_t duration_ms) {
    uint32_t period_us = 1000000 / frequency; 
    uint32_t half_period_us = period_us / 2; 
    uint32_t cycles = (duration_ms * 1000) / period_us; 
    for (uint32_t i = 0; i < cycles; i++) {
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8, GPIO_PIN_SET); 
        delay_us(half_period_us); 
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8, GPIO_PIN_RESET); 
        delay_us(half_period_us); 
    }
}