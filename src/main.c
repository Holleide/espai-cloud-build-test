#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "driver/gpio.h"

// --- 配置部分 ---
// 根据实际硬件配置，请在此处定义正确的GPIO编号。
// 示例：对于ESP32S3的板载LED可能需要查阅Datasheet或CubeMX配置。
#define LED_GPIO_NUM (GPIO_NUM_45) // 假设使用GPIO45作为输出

static const char *TAG = "main";

/**
 * @brief GPIO初始化函数。
 * @param gpio_num 要配置的GPIO编号。
 * @retval ESP_OK 表示成功，ESP_ERR_GPIO_INIT 表示失败。
 */
static esp_err_t gpio_init(gpio_num_t gpio_num)
{
    // 使用 esp_driver_gpio 相关的API进行GPIO初始化
    esp_err_t ret = esp_err_gpio_init(gpio_num, GPIO_MODE_OUTPUT, GPIO_DEFAULT);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "GPIO初始化失败，错误码: %d", ret);
        return ret; // 返回实际错误码
    }
    ESP_LOGI(TAG, "GPIO %d 初始化成功。", gpio_num);
    return ESP_OK;
}

/**
 * @brief LED控制任务，负责LED的非阻塞闪烁。
 * @param pvTaskWoken 任务被唤醒的事件信息。
 */
static void led_task(void *pvTaskWoken)
{
    ESP_LOGI(TAG, "LED控制任务启动。");

    // 1. 初始化GPIO
    if (gpio_init(LED_GPIO_NUM) != ESP_OK) {
        ESP_LOGE(TAG, "系统无法初始化LED GPIO，任务终止。");
        vTaskDelete(NULL); // 初始化失败则删除任务
        return;
    }

    while (1) {
        // 2. 设置LED为高电平 (点亮)
        esp_err_t ret = esp_err_gpio_set_level(LED_GPIO_NUM, 1);
        if (ret != ESP_OK) {
            ESP_LOGE(TAG, "设置GPIO %d 为高电平失败，错误码: %d", LED_GPIO_NUM, ret);
        } else {
            ESP_LOGI(TAG, "LED点亮。");
        }

        // 3. 非阻塞延时 (使用 FreeRTOS)
        vTaskDelay(pdMS_TO_TICKS(500));

        // 4. 设置LED为低电平 (熄灭)
        esp_err_t ret_off = esp_err_gpio_set_level(LED_GPIO_NUM, 0);
        if (ret_off != ESP_OK) {
            ESP_LOGE(TAG, "设置GPIO %d 为低电平失败，错误码: %d", LED_GPIO_NUM, ret_off);
        } else {
            ESP_LOGI(TAG, "LED熄灭。");
        }

        // 5. 非阻塞延时 (使用 FreeRTOS)
        vTaskDelay(pdMS_TO_TICKS(500));
    }
}

/**
 * @brief ESP-IDF 应用主入口点。
 */
void app_main(void)
{
    ESP_LOGI(TAG, "应用启动，开始执行LED示例。");

    // 创建一个任务来处理LED的闪烁逻辑
    // 栈大小设置为4096字节，足够容纳FreeRTOS上下文和代码栈。
    xTaskCreate(led_task, "led_task", 4096, NULL, 5, NULL);

    // 主线程保持运行，等待任务执行
    while (1) {
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}