#include <Arduino.h>

// 定义要控制的 GPIO 引脚
const int LED_PIN = 48;

void setup() {
  // 配置 GPIO48 为输出模式
  pinMode(LED_PIN, OUTPUT);
}

void loop() {
  // 设置 LED 为高电平 (HIGH)
  digitalWrite(LED_PIN, HIGH);
  delay(500);              // 延迟 500 毫秒

  // 设置 LED 为低电平 (LOW)
  digitalWrite(LED_PIN, LOW);
  delay(500);              // 延迟 500 毫秒
}
