#ifndef __SEG_h
#define __SEG_h

/*头文件引入*/
#include "main.h"
#include "gpio.h"
#include "basic.h"

#define RCK_PIN  GPIO_PIN_8
#define RCK_PORT GPIOA
#define SI_PIN   GPIO_PIN_7
#define SI_PORT  GPIOA
#define SCK_PIN  GPIO_PIN_5
#define SCK_PORT GPIOA


//段码
extern const uint8_t Seg_Table[];

void HC595_SendByte(uint8_t data);
void Seg_disp(uint8_t digit, uint8_t position);
void Seg_Init();

#endif 