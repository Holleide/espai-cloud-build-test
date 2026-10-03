/**
 * @file main.cpp
 * @brief STM32F103C8 LED 工业级控制示例
 * 
 * 实现功能：
 * - 非阻塞式 LED 闪烁（基于 millis() 状态机）
 * - 符合 MISRA C 规范的代码结构
 * - 完善的初始化与错误处理
 */

#include <Arduino.h>

// ======================== 配置常量 ========================
/** @brief STM32F103C8 (Black Pill) 板载 LED 引脚 PC13（低电平点亮） */
#define LED_PIN                     PC13

/** @brief LED 状态切换间隔时间（毫秒） */
#define LED_BLINK_INTERVAL_MS       500UL

// ======================== 类型定义 ========================

/** @brief LED 工作状态机枚举 */
typedef enum {
    STATE_LED_OFF = 0,      /**< LED 熄灭状态 */
    STATE_LED_ON            /**< LED 点亮状态 */
} led_state_t;

/** @brief 系统运行状态枚举 */
typedef enum {
    STATE_INIT = 0,         /**< 初始化阶段 */
    STATE_RUNNING           /**< 正常运行阶段 */
} system_state_t;

// ======================== 全局变量 ========================

/** @brief LED 当前状态（静态局部或全局，用于状态保持） */
static led_state_t s_led_state = STATE_LED_OFF;

/** @brief 上一次状态切换的时间戳 */
static uint32_t s_last_switch_time = 0U;

/** @brief 系统运行标志（volatile 确保跨上下文可见性） */
static volatile bool s_system_running = false;

// ======================== 函数声明 ========================

/**
 * @brief 硬件初始化函数
 * @return true 初始化成功，false 初始化失败
 */
static bool hw_init(void);

/**
 * @brief LED 状态更新函数（非阻塞）
 * @note 基于 millis() 实现精确延时，不阻塞主循环
 */
static void led_update(void);

// ======================== 函数实现 ========================

/**
 * @brief 硬件初始化
 * @return true 成功，false 失败
 */
static bool hw_init(void) {
    // 设置 LED 引脚为输出模式
    pinMode(LED_PIN, OUTPUT);
    
    // 初始状态：熄灭 LED（PC13 低电平点亮，故初始为 HIGH）
    digitalWrite(LED_PIN, HIGH);
    
    s_system_running = true;
    s_last_switch_time = millis();
    
    return true;
}

/**
 * @brief LED 非阻塞状态更新
 */
static void led_update(void) {
    uint32_t current_time = millis();
    
    // 检查是否到达切换时间点（防止 millis() 溢出导致的逻辑错误）
    if ((current_time - s_last_switch_time) >= LED_BLINK_INTERVAL_MS) {
        // 切换 LED 状态
        if (s_led_state == STATE_LED_OFF) {
            digitalWrite(LED_PIN, LOW);   // 点亮（低电平有效）
            s_led_state = STATE_LED_ON;
        } else {
            digitalWrite(LED_PIN, HIGH);  // 熄灭
            s_led_state = STATE_LED_OFF;
        }
        
        // 更新时间戳
        s_last_switch_time = current_time;
    }
}

// ======================== Arduino 入口函数 ========================

/**
 * @brief 系统初始化入口（仅执行一次）
 */
void setup() {
    // 硬件初始化
    if (!hw_init()) {
        // 初始化失败处理：持续快速闪烁 LED 表示故障
        while (true) {
            digitalWrite(LED_PIN, LOW);
            delay(100U);
            digitalWrite(LED_PIN, HIGH);
            delay(100U);
        }
    }
}

/**
 * @brief 系统主循环（无限重复执行）
 */
void loop() {
    // 仅在系统正常运行时执行业务逻辑
    if (s_system_running) {
        led_update();  // 非阻塞式 LED 状态更新
    }
}