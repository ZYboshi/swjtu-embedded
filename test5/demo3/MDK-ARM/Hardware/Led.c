#include "Led.h"

/*
  * @brief  控制所有LED状态
  * @param  state: 1-全亮, 0-全灭
  */
void LED_SetAll(uint8_t state) {
    if(state) {
        HAL_GPIO_WritePin(LED_PORT, LED_ALL_PINS, GPIO_PIN_SET);
    } else {
        HAL_GPIO_WritePin(LED_PORT, LED_ALL_PINS, GPIO_PIN_RESET);
    }
}

/*
  * @brief  跑马灯效果
  * @param  direction: 1-顺时针, 0-逆时针
  * @param  speed_ms: 每个LED的显示时间(毫秒)
  */
void LED_RunningLight(uint8_t direction, uint16_t speed_ms) {
    static const uint16_t led_pins[8] = {
        GPIO_PIN_0, GPIO_PIN_1, GPIO_PIN_2, GPIO_PIN_3,
        GPIO_PIN_4, GPIO_PIN_5, GPIO_PIN_6, GPIO_PIN_7
    };
    static uint8_t current_pos = 0;  // 静态变量记录当前位置

    if(direction) { // 顺时针右移
        current_pos = (current_pos + 1) % 8;  // 位置右移
    } else {        // 逆时针左移
        current_pos = (current_pos == 0) ? 7 : (current_pos - 1);  // 位置左移
    }

    // 点亮当前LED
    HAL_GPIO_WritePin(LED_PORT, LED_ALL_PINS, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(LED_PORT, led_pins[current_pos], GPIO_PIN_SET);
    delay_ms(speed_ms);
}