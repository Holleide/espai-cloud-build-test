#include <Arduino.h>

// 由 EspAiStudio 从其它框架转换而来的 Arduino 入口占位。
// 若原工程逻辑未自动迁入，请在此按需求补写 setup/loop，或从 .esp-ai-converted 备份目录取回原代码改写。
#ifndef LED_BUILTIN
#define LED_BUILTIN 2
#endif

void setup() {
    pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
    digitalWrite(LED_BUILTIN, HIGH);
    delay(500);
    digitalWrite(LED_BUILTIN, LOW);
    delay(500);
}
