#include "Key.h"


/**
  * @brief  读取按键状态
  * @retval uint8_t 
  *         0: 无按键按下
  *         1: KB1按下
  *         2: KB2按下
  *         3: KB3按下
  *         4: KB4按下
  */
uint8_t key_read(void)
{
    uint8_t temp = 0;
    
    if (HAL_GPIO_ReadPin(KB1_GPIO_Port, KB1_Pin) == GPIO_PIN_RESET) {
        temp = 1;
    }
    if (HAL_GPIO_ReadPin(KB2_GPIO_Port, KB2_Pin) == GPIO_PIN_RESET) {
        temp = 2;
    }
    if (HAL_GPIO_ReadPin(KB3_GPIO_Port, KB3_Pin) == GPIO_PIN_RESET) {
        temp = 3;
    }
    if (HAL_GPIO_ReadPin(KB4_GPIO_Port, KB4_Pin) == GPIO_PIN_RESET) {
        temp = 4;
    }
    
    return temp;
}