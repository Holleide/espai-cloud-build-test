/*
 * @file main.cpp - STM32F103C8 LED Blink Example (PC13)
 */

#include <Arduino.h>
#include "led_control.h"

/**
 * @brief PC13 → GPIOC^13 → Arduino-LED-BUILTIN（STM32F103C8 板载）
 */
const int ledPin = 13;

void setup() {
    // 初始化串口调试输出（115200 bps，8N1）
    Serial.begin(115200);
    while (!Serial) {
        ; // 等待串口连接（仅用于调试）
    }
    Serial.println("STM32F103C8 LED Blink - PC13");

    // 初始化 LED 控制模块
    led_init();

    // 初始状态：LED 关闭（LOW）
    led_off();

    Serial.println("Setup complete - LED will blink (approx. 500ms period)");
}

void loop() {
    /**
     * @brief 非阻塞式 LED 闪烁（500ms 周期）
     * 
     * 使用 led_blink() 函数实现非阻塞式周期性翻转，避免占用主循环资源。
     */
    led_blink();
}

// 静态变量：记录上一次闪烁时间（非阻塞）
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

