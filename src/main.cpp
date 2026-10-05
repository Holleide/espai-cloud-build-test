#include <Arduino.h>

// 定义要控制的 GPIO 引脚
const int LED_PIN = 48;

void setup() {
    // 配置 GPIO48 为输出模式
    pinMode(LED_PIN, OUTPUT);
}

void loop() {
    // 设置 GPIO48 为高电平 (点亮 LED)
    digitalWrite(LED_PIN, HIGH);
    delay(500); // 延时 500 毫秒

    // 设置 GPIO48 为低电平 (熄灭 LED)
    digitalWrite(LED_PIN, LOW);
    delay(500); // 延时 500 毫秒
}