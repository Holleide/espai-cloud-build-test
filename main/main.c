/* ==============================================================================
 *  作        者：EspAiStudio · AI 代码工程师
 *  功 能 简 介：主程序入口源文件（工程：hhh）
 * ============================================================================== */

/* ==============================================================================
 *  作        者：EspAiStudio · AI 代码工程师
 *  功 能 简 介：点灯 Demo - ESP-IDF GPIO 翻转 LED（工程：hhh）
 * ============================================================================== */

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"

#define LED_GPIO_NUM    2

void app_main(void) {
    /* ========== 初始化 ========== */
    gpio_config_t cfg = GPIO_CONFIG_MODE;          // 输入输出模式
    cfg.intr_type = GPIO_INTR_DISABLE;            // 禁用中断
    cfg.pin = LED_GPIO_NUM;                       // LED GPIO 引脚号（ESP32-C3）
    cfg.mode = GPIO_MODE_OUTPUT;                  // 设置为输出
    cfg.pull_up_en = GPIO_PULLUP_DISABLE;         // 不使用上拉电阻
    cfg.pull_down_en = GPIO_PULLDOWN_DISABLE;     // 不使用下拉电阻

    gpio_config(&cfg);                             // 配置 GPIO

    /* ========== 主循环 ========== */
    while (1) {
        gpio_set_level(LED_GPIO_NUM, 1);           // 点亮 LED（高电平）
        vTaskDelay(pdMS_TO_TICKS(500));            // 延时 500ms
        gpio_set_level(LED_GPIO_NUM, 0);           // 关闭 LED（低电平）
        vTaskDelay(pdMS_TO_TICKS(500));            // 延时 500ms
    }
}
