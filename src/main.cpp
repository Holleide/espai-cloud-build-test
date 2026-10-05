#include <Arduino.h>

// 定义要控制的 GPIO 引脚，根据需求是 GPIO48
const int LED_PIN = 48;

void setup() {
    // 配置 GPIO48 为输出模式
    pinMode(LED_PIN, OUTPUT);
    Serial.begin(115200);
    Serial.println("GPIO48 LED Blink Test Initialized.");
}

void loop() {
    // 闪烁逻辑：高电平 500ms，低电平 500ms
    digitalWrite(LED_PIN, HIGH);
    delay(500);
    digitalWrite(LED_PIN, LOW);
    delay(500);
}