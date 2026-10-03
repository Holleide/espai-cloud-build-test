#include <Arduino.h>

// STM32F103C8 (Black Pill) 板载 LED 引脚为 PC13（低电平点亮）
#define LED_PIN PC13

void setup() {
    // 初始化 LED 引脚为输出模式
    pinMode(LED_PIN, OUTPUT);
}

void loop() {
    digitalWrite(LED_PIN, LOW);   // 点灯（PC13 低电平点亮）
    delay(500);                   // 延时 500ms
    digitalWrite(LED_PIN, HIGH);  // 灭灯
    delay(500);                   // 延时 500ms
}