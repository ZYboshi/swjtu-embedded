#include "Seg.h"


//段码
/*特殊断码补充: '-' 0x40 
*/
const uint8_t Seg_Table[] = {
    // 0-9
    0x3F, // 0 (a-b-c-d-e-f)
    0x06, // 1 (b-c)
    0x5B, // 2 (a-b-g-e-d)
    0x4F, // 3 (a-b-g-c-d)
    0x66, // 4 (f-g-b-c)
    0x6D, // 5 (a-f-g-c-d)
    0x7D, // 6 (a-f-g-e-c-d)
    0x07, // 7 (a-b-c)
    0x7F, // 8 (a-b-c-d-e-f-g)
    0x6F, // 9 (a-b-c-d-f-g)

    // A-F（16进制）
    0x77, // A (a-b-c-e-f-g)
    0x7C, // b (f-g-e-d-c)
    0x39, // C (a-d-e-f)
    0x5E, // d (b-c-d-e-g)
    0x79, // E (a-d-e-f-g)
};

void HC595_SendByte(uint8_t data) {
    for (uint8_t i = 0; i < 8; i++) {
        // 先移出最高位（MSB First）
        HAL_GPIO_WritePin(SI_PORT, SI_PIN, (data & (0x80 >> i)) ? GPIO_PIN_SET : GPIO_PIN_RESET);
        
        // 产生SCK上升沿（移位）
        HAL_GPIO_WritePin(SCK_PORT, SCK_PIN, GPIO_PIN_SET);
        HAL_GPIO_WritePin(SCK_PORT, SCK_PIN, GPIO_PIN_RESET);
    }
    
    // 产生RCK上升沿（锁存输出）
    HAL_GPIO_WritePin(RCK_PORT, RCK_PIN, GPIO_PIN_SET);
    HAL_GPIO_WritePin(RCK_PORT, RCK_PIN, GPIO_PIN_RESET);
}

void Seg_disp(uint8_t digit, uint8_t position) {
    // 1. 发送段码到74HC595
    HC595_SendByte(digit);

    HAL_GPIO_WritePin(GPIOF, GPIO_PIN_15 >> (position-1), GPIO_PIN_RESET); // 打开位选
    delay_ms(1); // 短暂延时（防闪烁）
    HAL_GPIO_WritePin(GPIOF, GPIO_PIN_15 >> (position-1), GPIO_PIN_SET); // 关闭位选
}
void Seg_Init(){
    HAL_GPIO_WritePin(GPIOF, GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10 | GPIO_PIN_11 |
                             GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15, 
                             GPIO_PIN_SET);
}