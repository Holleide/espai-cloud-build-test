#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "esp_log.h"

// 定义 LED 连接的 GPIO 引脚（根据目标芯片，假设使用板载 LED 或指定引脚）
#define LED_GPIO_NUM GPIO_NUM_2 // 假设连接到 GPIO 2 (常见的板载LED)

static const char *TAG = "led_blink";

/**
 * @brief 配置 GPIO 引脚作为输出
 * @param gpio_num 要配置的 GPIO 号
 */
static esp_err_t gpio_init(gpio_num_t gpio_num)
{
    // 初始化 GPIO 驱动
    esp_err_t ret = gpio_set_direction(gpio_num, GPIO_MODE_OUTPUT);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "GPIO 设置方向失败: %d", ret);
        return ret;
    }

    // 初始化 LED 状态（设置为低电平，关闭 LED）
    gpio_set_level(gpio_num, 0);
    return ESP_OK;
}

/**
 * @brief 主任务：实现 LED 的闪烁逻辑
 */
static void led_task(void *pvParameters)
{
    // 初始化 GPIO
    if (gpio_init(LED_GPIO_NUM) != ESP_OK) {
        ESP_LOGE(TAG, "GPIO 初始化失败，任务终止。");
        vTaskDelete(NULL); // 失败则删除任务
    }

    while (1) {
        // 点亮 LED
        gpio_set_level(LED_GPIO_NUM, 1);
        ESP_LOGI(TAG, "LED 点亮");
        vTaskDelay(pdMS_TO_TICKS(500)); // 延时 500ms

        // 熄灭 LED
        gpio_set_level(LED_GPIO_NUM, 0);
        ESP_LOGI(TAG, "LED 熄灭");
        vTaskDelay(pdMS_TO_TICKS(500)); // 延时 500ms
    }
}

/**
 * @brief 应用主函数，作为 ESP-IDF 的入口点
 */
void app_main(void)
{
    ESP_LOGI(TAG, "Application main started.");

    // 创建 LED 闪烁任务，优先级可以根据需要调整
    xTaskCreate(led_task, "led_task", 4096, NULL, 5, NULL);

    // 主线程等待任务执行（在 FreeRTOS 环境中通常是必要的）
    vTaskDelay(pdMS_TO_TICKS(100));
}