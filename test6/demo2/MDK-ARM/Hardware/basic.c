#include "basic.h"

void delay_us(uint32_t us) {
    uint32_t ticks = us * (SystemCoreClock / 5000000);
    while(ticks--) {
        __ASM volatile ("nop");  // �����ָ�֤�����ȶ�[4](@ref)
    }
}

void delay_ms(uint32_t ms) {
    while (ms--) {
        delay_us(1000);
    }
}