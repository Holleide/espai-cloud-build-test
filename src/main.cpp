#include <Arduino.h>

// 定义要控制的LED引脚为GPIO48
const int LED_PIN = 48;

void setup() {
    // 配置GPIO48为输出模式
    pinMode(LED_PIN, OUTPUT);
}

void loop() {
    // 点亮LED
    digitalWrite(LED_PIN, HIGH);
    delay(500); // 延迟500毫秒

    // 熄灭LED
    digitalWrite(LED_PIN, LOW);
    delay(500); // 延迟500毫秒
}