#include <Arduino.h> // 引入 Arduino 标准库

// 定义板载 LED 引脚
const int ledPin = LED_BUILTIN;

// --- 非阻塞状态机变量 ---
unsigned long previousMillis = 0; // 用于存储上一次执行时间
const long interval = 1000;     // 定时间隔 (1000ms)

/**
 * @brief 初始化函数，在程序启动时只执行一次。
 */
void setup() {
    pinMode(ledPin, OUTPUT); 
}

/**
 * @brief 主循环，无限重复执行。采用非阻塞模式控制 LED 闪烁。
 */
void loop() {
    unsigned long currentMillis = millis(); // 获取当前时间

    // 判断是否达到设定的定时间隔 (1000ms)
    if (currentMillis - previousMillis >= interval) {
        // 更新上一次执行的时间戳
        previousMillis = currentMillis; 

        // 执行 LED 的切换逻辑（状态机核心）
        digitalWrite(ledPin, !digitalRead(ledPin)); // 读取当前状态并取反
    }
}