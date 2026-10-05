// File: main/main.c
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "driver/gpio.h"

/* 模块标签，用于日志输出 */
static const char *TAG = "main";

/**
 * @brief GPIO 引脚定义和初始化结构体
 * 包含所有需要配置的引脚信息。
 */
typedef struct {
    gpio_num_t led_pin;       // LED 输出引脚编号
    gpio_num_t button_pin;   // 按键输入引脚编号
} gpio_config_t;

/**
 * @brief GPIO 配置结构体，用于存放硬件配置。
 */
static gpio_config_t gpio_cfg;

/**
 * @brief 任务函数：LED 点灯控制
 * 使用 FreeRTOS 任务来非阻塞地控制 LED。
 *
 * @param pvParameters 任务参数
 * @return void
 */
void led_task(void *pvParameters) {
    ESP_LOGI(TAG, "LED Control Task started.");

    // 假设我们将 LED 配置为 GPIO 2，按键配置为 GPIO 35 (示例引脚，需根据实际硬件调整)
    const gpio_num_t led_gpio = GPIO_NUM_2;
    const gpio_num_t button_gpio = GPIO_NUM_35;

    // 1. 初始化 LED 引脚为输出模式
    ESP_ERROR_CHECK(gpio_set_direction(led_gpio, GPIO_MODE_OUTPUT));
    ESP_LOGI(TAG, "LED GPIO %d initialized as output.", led_gpio);

    // 2. 初始化按键引脚为输入模式，并配置上拉/下拉（示例）
    ESP_ERROR_CHECK(gpio_set_direction(button_gpio, GPIO_MODE_INPUT));
    ESP_LOGI(TAG, "Button GPIO %d initialized as input.", button_gpio);

    // 3. 配置 LED 输出 (这里使用简单的 digitalWrite，实际项目中应使用 LEDC/PWM)
    while (1) {
        // 模拟点灯效果：开灯 500ms，关灯 500ms
        gpio_set_level(led_gpio, 1); // 点亮 LED
        ESP_LOGI(TAG, "LED ON.");
        vTaskDelay(pdMS_TO_TICKS(500));

        gpio_set_level(led_gpio, 0); // 熄灭 LED
        ESP_LOGI(TAG, "LED OFF.");
        vTaskDelay(pdMS_TO_TICKS(500));
    }
}

/**
 * @brief 应用主入口函数。
 *
 * 在 ESP-IDF 框架下，app_main 是程序执行的起点。
 * 此处负责系统初始化和任务创建。
 *
 * @param () 无参数
 */
void app_main(void) {
    ESP_LOGI(TAG, "Application main started.");

    // 启动 LED 控制任务
    ESP_ERROR_CHECK(xTaskCreate(led_task, "led_task", 4096, NULL, 5, NULL));

    ESP_LOGI(TAG, "System initialized. Waiting for tasks...");

    // 主循环保持运行，等待其他事件或任务完成
    while (1) {
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}