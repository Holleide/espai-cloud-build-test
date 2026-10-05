#include <Arduino.h>

// 定义 GPIO48 引脚为输出
const int LED_PIN = 48;

void setup() {
    // 初始化 GPIO48 为输出模式
    pinMode(LED_PIN, OUTPUT);
    // 初始化串口用于调试，如果需要
    Serial.begin(115200);
}

void loop() {
    // LED 闪烁逻辑：高电平 500ms，低电平 500ms
    digitalWrite(LED_PIN, HIGH);
    delay(500);
    digitalWrite(LED_PIN, LOW);
    delay(500);
}