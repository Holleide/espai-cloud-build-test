/**
 * @file main/main.c
 * @brief Esp32-S3 点灯与按键示例入口文件
 */

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "driver/gpio.h"

// 定义日志标签
static const char *TAG = "main";

// --- GPIO 引脚定义 ---
// 假设 LED 连接到 GPIO 45 (根据当前环境描述)
#define LED_GPIO_NUM    45
// 假设按键连接到 GPIO 35 (需根据实际硬件配置调整)
#define BUTTON_GPIO_NUM 35

/**
 * @brief 控制LED状态的FreeRTOS任务
 * 使用任务来避免阻塞主循环，实现非阻塞点灯。
 * @param pvParameters 传递给任务的参数
 */
void led_task(void *pvParameters)
{
    // 初始化LED引脚
    gpio_reset_pin(LED_GPIO_NUM);
    gpio_set_direction(LED_GPIO_NUM, GPIO_MODE_OUTPUT);

    ESP_LOGI(TAG, "LED Task started.");

    while (1) {
        // 闪烁LED
        gpio_set_level(LED_GPIO_NUM, 1);
        vTaskDelay(pdMS_TO_TICKS(500)); // 延时500ms

        gpio_set_level(LED_GPIO_NUM, 0);
        vTaskDelay(pdMS_TO_TICKS(500)); // 延时500ms
    }
}

/**
 * @brief 读取按键状态的FreeRTOS任务
 * 监控按键输入。
 * @param pvParameters 传递给任务的参数
 */
void button_task(void *pvParameters)
{
    ESP_LOGI(TAG, "Button Task started.");
    
    // 初始化按键引脚为输入，并配置上拉/下拉（此处假设使用内部上拉）
    gpio_reset_pin(BUTTON_GPIO_NUM);
    gpio_set_direction(BUTTON_GPIO_NUM, GPIO_MODE_INPUT);
    
    // 启用内部上拉电阻以简化按键连接 (需要根据实际硬件调整)
    gpio_pullup_en(BUTTON_GPIO_NUM);

    while (1) {
        int button_state = gpio_get_level(BUTTON_GPIO_NUM);
        ESP_LOGI(TAG, "Button state: %d", button_state);

        // 简单的按键去抖/事件处理逻辑
        if (button_state == 0) { // 按下（低电平）
            ESP_LOGW(TAG, "Button pressed!");
            vTaskDelay(pdMS_TO_TICKS(200)); // 去抖
        }
        vTaskDelay(pdMS_TO_TICKS(10)); // 轮询间隔
    }
}

/**
 * @brief 主应用程序入口函数
 */
void app_main(void)
{
    ESP_LOGI(TAG, "Application starting up...");

    // 创建LED控制任务
    xTaskCreate(led_task, "led_task", 4096, NULL, 5, NULL);

    // 创建按键输入任务
    xTaskCreate(button_task, "button_task", 4096, NULL, 6, NULL);

    ESP_LOGI(TAG, "All tasks created successfully.");
}