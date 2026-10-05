#include <Arduino.h>

// 定义 GPIO48 引脚为输出
const int ledPin = 48;

void setup() {
    // 配置 GPIO48 为输出模式
    pinMode(ledPin, OUTPUT);
    Serial.begin(115200);
    Serial.println("GPIO48 LED Blink Test Started");
}

void loop() {
    // 点亮 LED
    digitalWrite(ledPin, HIGH);
    Serial.println("LED ON");
    delay(500); // 延时 500 毫秒

    // 熄灭 LED
    digitalWrite(ledPin, LOW);
    Serial.println("LED OFF");
    delay(500); // 延时 500 毫秒
}