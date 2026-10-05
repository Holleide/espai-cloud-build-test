/**
 * @file main/main.c
 * @brief ESP32-S3 LED 点灯示例
 */

#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "driver/gpio.h" // 引入 GPIO 驱动头文件

static const char *TAG = "main";

// 定义 LED 连接的 GPIO 引脚。根据 ESP32-S3 的硬件引脚进行选择。
// 假设我们使用一个常见的板载 LED 引脚，例如 GPIO45 或其他可用的 GPIO。
// 请根据实际硬件连接修改此值。这里示例使用 GPIO45。
#define LED_GPIO_NUM GPIO45

/**
 * @brief 初始化 GPIO 引脚配置
 * @param gpio_num 要配置的 GPIO 引脚号
 */
static esp_err_t gpio_init(gpio_num_t gpio_num) {
    // 使用 esp_err_t 检查函数调用结果，遵循错误处理准则 (E01/E02)
    ESP_LOGI(TAG, "Initializing GPIO: %d", gpio_num);

    // 配置 GPIO 引脚的入/输出模式
    gpio_config_t io_config;
    io_config.mode = GPIO_MODE_OUTPUT; // 设置为输出模式
    io_config.pull_up_en = GPIO_PULLUP_DISABLE; // 关闭上拉电阻
    io_config.pull_down_en = GPIO_PULLDOWN_DISABLE; // 关闭下拉电阻

    // 配置引脚的输出模式
    ESP_ERROR_CHECK(gpio_config(&gpio_num, &io_config));

    ESP_LOGI(TAG, "GPIO %d initialized successfully.", gpio_num);
    return ESP_OK;
}


/**
 * @brief 主应用任务函数
 * @param pvParameters 任务参数
 */
static void led_task(void *pvParameters) {
    // 初始化 LED GPIO
    ESP_ERROR_CHECK(gpio_init(LED_GPIO_NUM));

    while (1) {
        // 点亮 LED
        ESP_LOGI(TAG, "LED is ON");
        gpio_set_level(LED_GPIO_NUM, 1); // 设置引脚为高电平

        // 等待一段时间（非阻塞，使用 FreeRTOS 延时）
        vTaskDelay(pdMS_TO_TICKS(500));

        // 熄灭 LED
        ESP_LOGI(TAG, "LED is OFF");
        gpio_set_level(LED_GPIO_NUM, 0); // 设置引脚为低电平

        // 等待一段时间
        vTaskDelay(pdMS_TO_TICKS(500));
    }
}


/**
 * @brief 应用主入口函数
 */
void app_main(void) {
    ESP_LOGI(TAG, "Application started.");

    // 创建一个任务来执行 LED 点灯逻辑
    xTaskCreate(led_task, "led_task", 4096, NULL, 5, NULL);

    ESP_LOGI(TAG, "app_main finished initialization. Task created.");
}