#ifndef __BUZZER_h
#define __BUZZER_h

#include "main.h"
#include "gpio.h"
#include "basic.h"

#define BUZZER_PIN  GPIO_PIN_8
#define BUZZER_PORT GPIOB

void PlayBuzzerTone(uint32_t frequency, uint32_t duration_ms);

#endif 