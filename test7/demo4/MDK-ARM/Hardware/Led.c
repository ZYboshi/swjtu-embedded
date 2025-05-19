#include "Led.h"

/*
  * @brief  ��������LED״̬
  * @param  state: 1-ȫ��, 0-ȫ��
  */
void LED_SetAll(uint8_t state) {
    if(state) {
        HAL_GPIO_WritePin(LED_PORT, LED_ALL_PINS, GPIO_PIN_SET);
    } else {
        HAL_GPIO_WritePin(LED_PORT, LED_ALL_PINS, GPIO_PIN_RESET);
    }
}

/*
  * @brief  �����Ч��
  * @param  direction: 1-˳ʱ��, 0-��ʱ��
  * @param  speed_ms: ÿ��LED����ʾʱ��(����)
  */
void LED_RunningLight(uint8_t direction, uint16_t speed_ms) {
    static const uint16_t led_pins[8] = {
        GPIO_PIN_0, GPIO_PIN_1, GPIO_PIN_2, GPIO_PIN_3,
        GPIO_PIN_4, GPIO_PIN_5, GPIO_PIN_6, GPIO_PIN_7
    };
    static uint8_t current_pos = 0;  // ��̬������¼��ǰλ��

    if(direction) { // ˳ʱ������
        current_pos = (current_pos + 1) % 8;  // λ������
    } else {        // ��ʱ������
        current_pos = (current_pos == 0) ? 7 : (current_pos - 1);  // λ������
    }

    // ������ǰLED
    HAL_GPIO_WritePin(LED_PORT, LED_ALL_PINS, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(LED_PORT, led_pins[current_pos], GPIO_PIN_SET);
    delay_ms(speed_ms);
}