#ifndef __LED_h
#define __LED_h

#include "main.h"
#include "gpio.h"
#include "basic.h"


#define LED_ALL_PINS (GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 | \
                     GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7)
#define LED_PORT GPIOF
void LED_SetAll(uint8_t state);
void LED_RunningLight(uint8_t direction, uint16_t speed_ms) ;

#endif 