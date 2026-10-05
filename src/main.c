#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "driver/gpio.h"

static const char *TAG = "main";

/**
 * @brief LED引脚的GPIO编号。
 *        根据目标芯片和硬件连接，需要根据实际硬件配置来确定。
 *        此处假设使用板载LED，具体引脚需查阅 esp32s3 的引脚定义。
 */
#define LED_GPIO_NUM GPIO45 // 示例：假设点亮的是GPIO45

/**
 * @brief 初始化GPIO的函数。
 * @param gpio_num 要配置的GPIO编号。
 */
static void gpio_init(gpio_num_t gpio_num)
{
    // 使用 esp_driver_gpio 相关的API进行GPIO初始化，v6.x 版本推荐使用 driver 组件接口
    esp_err_t ret = esp_err_gpio_init(gpio_num, GPIO_MODE_OUTPUT, GPIO_DEFAULT);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "GPIO初始化失败，错误码: %d", ret);
        // 错误处理：在实际应用中应采取更严格的错误上报或系统复位措施。
        return;
    }
    ESP_LOGI(TAG, "GPIO %d 初始化成功。", gpio_num);
}

/**
 * @brief 主应用任务，负责LED的闪烁。
 * @param pvTaskWoken 任务被唤醒的事件信息。
 */
static void led_task(void *pvTaskWoken)
{
    ESP_LOGI(TAG, "LED控制任务启动。");

    // 初始化LED引脚
    gpio_init(LED_GPIO_NUM);

    while (1) {
        // 设置LED为高电平 (点亮)
        esp_err_t ret = esp_err_gpio_set_level(LED_GPIO_NUM, 1);
        if (ret != ESP_OK) {
            ESP_LOGE(TAG, "设置GPIO %d 为高电平失败，错误码: %d", LED_GPIO_NUM, ret);
        } else {
            ESP_LOGI(TAG, "LED点亮。");
        }

        // 延时一段时间 (使用 FreeRTOS 提供的延时函数)
        vTaskDelay(pdMS_TO_TICKS(500));

        // 设置LED为低电平 (熄灭)
        esp_err_t ret_off = esp_err_gpio_set_level(LED_GPIO_NUM, 0);
        if (ret_off != ESP_OK) {
            ESP_LOGE(TAG, "设置GPIO %d 为低电平失败，错误码: %d", LED_GPIO_NUM, ret_off);
        } else {
            ESP_LOGI(TAG, "LED熄灭。");
        }

        // 延时一段时间 (使用 FreeRTOS 提供的延时函数)
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
    xTaskCreate(led_task, "led_task", 4096, NULL, 5, NULL);

    // 在主循环中保持系统运行（如果任务已创建，通常不需要在 app_main 中阻塞）
    while (1) {
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}