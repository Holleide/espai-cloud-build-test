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

    // 配置引脚：假设 LED 连接到 GPIO 2，按键连接到 GPIO 35 (示例引脚，需根据实际硬件调整)
    const gpio_num_t led_gpio = GPIO_NUM_2;
    const gpio_num_t button_gpio = GPIO_NUM_35;

    // 1. 初始化 LED 引脚为输出模式
    ESP_LOGI(TAG, "Attempting to configure LED GPIO %d as output.", led_gpio);
    if (gpio_set_direction(led_gpio, GPIO_MODE_OUTPUT) != ESP_OK) {
        ESP_LOGE(TAG, "Failed to set direction for LED GPIO %d. Error: %d", led_gpio, esp_err_to_name(esp_err_t));
        vTaskDelete(NULL); // 发生错误则删除任务并退出
    } else {
        ESP_LOGI(TAG, "LED GPIO %d initialized as output successfully.", led_gpio);
    }

    // 2. 初始化按键引脚为输入模式，并配置上拉（示例）
    ESP_LOGI(TAG, "Attempting to configure Button GPIO %d as input.", button_gpio);
    if (gpio_set_direction(button_gpio, GPIO_MODE_INPUT) != ESP_OK) {
        ESP_LOGE(TAG, "Failed to set direction for Button GPIO %d. Error: %d", button_gpio, esp_err_to_name(esp_err_t));
        vTaskDelete(NULL); // 发生错误则删除任务并退出
    } else {
        ESP_LOGI(TAG, "Button GPIO %d initialized as input successfully.", button_gpio);
    }

    // 3. 主循环：LED 周期性点亮/熄灭
    while (1) {
        // 模拟点灯效果：开灯 500ms，关灯 500ms
        ESP_LOGI(TAG, "Ticking: LED ON.");
        gpio_set_level(led_gpio, 1); // 点亮 LED
        vTaskDelay(pdMS_TO_TICKS(500));

        ESP_LOGI(TAG, "Ticking: LED OFF.");
        gpio_set_level(led_gpio, 0); // 熄灭 LED
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

    // --- 系统级初始化 (解决CRITICAL 2：GPIO在启动阶段完成) ---
    const gpio_num_t led_gpio = GPIO_NUM_2;
    const gpio_num_t button_gpio = GPIO_NUM_35;

    // 配置 LED 引脚为输出模式
    if (gpio_set_direction(led_gpio, GPIO_MODE_OUTPUT) != ESP_OK) {
        ESP_LOGE(TAG, "System Fatal Error: Failed to configure LED GPIO %d. Error: %d", led_gpio, esp_err_to_name(esp_err_t));
        return; // 发生致命错误则退出 app_main
    }
    ESP_LOGI(TAG, "System initialized: LED GPIO %d configured as output.", led_gpio);

    // 初始化按键引脚为输入模式，并配置上拉（示例）
    if (gpio_set_direction(button_gpio, GPIO_MODE_INPUT) != ESP_OK) {
        ESP_LOGE(TAG, "System Fatal Error: Failed to configure Button GPIO %d. Error: %d", button_gpio, esp_err_to_name(esp_err_t));
        return; // 发生致命错误则退出 app_main
    }
    ESP_LOGI(TAG, "System initialized: Button GPIO %d configured as input.", button_gpio);


    // --- 启动任务 (解决MAJOR 4：日志记录和错误检查) ---
    ESP_LOGI(TAG, "Starting LED Control Task...");
    if (xTaskCreate(led_task, "led_task", 4096, NULL, 5, NULL) != pdPASS) {
        ESP_LOGE(TAG, "Failed to create LED Control Task. Error: %d", xTaskCreate(led_task, "led_task", 4096, NULL, 5, NULL));
    } else {
        ESP_LOGI(TAG, "LED Control Task created successfully.");
    }

    ESP_LOGI(TAG, "System initialization complete. Application running in main loop.");

    // 主循环保持运行，等待其他事件或任务完成
    while (1) {
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}