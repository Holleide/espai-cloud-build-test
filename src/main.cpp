#include <Arduino.h> // 引入 Arduino 标准库

// 定义板载 LED 引脚，对于 Arduino Uno 默认是 13 号引脚 (LED_BUILTIN)
const int ledPin = LED_BUILTIN; // 使用宏定义确保跨平台兼容性

/**
 * @brief 初始化函数，在程序启动时只执行一次。
 */
void setup() {
    // 配置 LED 引脚为输出模式
    pinMode(ledPin, OUTPUT); 
}

/**
 * @brief 主循环，无限重复执行。
 */
void loop() {
    // 打开 LED (设置为 HIGH)
    digitalWrite(ledPin, HIGH);
    delay(1000); // 等待 1 秒

    // 关闭 LED (设置为 LOW)
    digitalWrite(ledPin, LOW);
    delay(1000); // 等待 1 秒
}