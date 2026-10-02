// 由 ESP-AI Studio 生成的 PlatformIO 工程
// 本地编辑，云端编译（GitHub Actions：pio run）
#include <Arduino.h>

/* 板载 LED 引脚：STM32F103C8 BluePill 的 onboard LED 位于 PC13 */
const uint8_t ledPin = PC13;

void setup() {
    pinMode(ledPin, OUTPUT);
}

void loop() {
    digitalWrite(ledPin, !digitalRead(ledPin)); /* 翻转当前电平 */
    delay(100);                                  /* 每 300 ms 翻转一次 */
}
