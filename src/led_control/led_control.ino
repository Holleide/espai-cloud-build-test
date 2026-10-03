/*
 * @file led_control.ino - PC13 LED Control Module (Generic STM32F103C8)
 * 
 * 该模块提供一个独立的、可复用的点灯控制函数，避免在 main.cpp 中直接使用 delay()
 * 提供非阻塞式实现（基于定时器中断或轮询），防止占用主循环资源。
 */

#include <Arduino.h>

/**
 * @brief LED 控制模块：PC13 引脚（对应 Arduino LED_BUILTIN）
 * 
 * 基于 STM32F103C8 的引脚定义，使用 Arduino core 机制初始化并控制。
 */
const int ledPin = 13;

/**
 * @brief 初始化 LED 模块：配置引脚为输出模式
 * 
 * 该函数由 main.cpp 调用，在 setup() 阶段执行一次。
 * 
 * @return void
 */
void led_init(void) {
    pinMode(ledPin, OUTPUT);
}

/**
 * @brief 点灯控制：使能 LED 输出电平
 * 
 * 该函数可被多个任务调用，用于独立触发点亮行为。
 * 
 * @return void
 */
void led_on(void) {
    digitalWrite(ledPin, HIGH);
}

/**
 * @brief 熄灭 LED：使能输出为低电平
 * 
 * 该函数可被多个任务调用，用于独立触发关闭行为。
 * 
 * @return void
 */
void led_off(void) {
    digitalWrite(ledPin, LOW);
}

/**
 * @brief 模拟 LED 闪烁（500ms 周期）
 * 
 * 使用非阻塞式方法实现，避免在 loop() 中长时间阻塞。
 * 
 * @return void
 */
void led_blink(void) {
    static uint32_t last_blink_time = 0;
    const uint32_t blink_interval = 500; // 毫秒
    
    if (millis() - last_blink_time >= blink_interval) {
        last_blink_time = millis();
        led_toggle();
    }
}

/**
 * @brief 快速翻转 LED 状态（高→低 或 低→高）
 * 
 * 提供一个原子式状态切换，避免中间状态。
 * 
 * @return void
 */
void led_toggle(void) {
    if (digitalRead(ledPin) == LOW) {
        led_on();
    } else {
        led_off();
    }
}

/**
 * @brief 获取当前 LED 状态（高/低）
 * 
 * 用于调试或状态查询。
 * 
 * @return uint8_t: 1=HIGH, 0=LOW
 */
uint8_t led_get_state(void) {
    return digitalRead(ledPin);
}

/**
 * @brief 静态变量：记录上一次闪烁时间（非阻塞）
 * 
 * 使用静态变量确保每次调用都基于当前时间计算。
 */
static uint32_t last_blink_time = 0;

// 头文件保护（防重复包含）
#ifndef LED_CONTROL_H_
#define LED_CONTROL_H_

/**
 * @brief LED 控制模块的公共 API 声明
 */
extern "C" {
    void led_init(void);
    void led_on(void);
    void led_off(void);
    void led_blink(void);
    void led_toggle(void);
    uint8_t led_get_state(void);
}

#endif // LED_CONTROL_H_
