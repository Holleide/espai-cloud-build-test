#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "driver/gpio.h"

static const char *TAG = "main";

/**
 * @brief 初始化GPIO引脚作为输出。
 * @param gpio_num 要配置的GPIO编号。
 */
static void gpio_init(gpio_num_t gpio_num)
{
    // 配置GPIO引脚的模式为输出
    gpio_config_t io_config;
    gpio_config(&io_config);
    io_config.mode = GPIO_MODE_OUTPUT;
    io_config.pin_bit_mask = (1ULL << gpio_num);
    io_config.pull_up_en = GPIO_PULLUP_DISABLE;
    io_config.pull_down_en = GPIO_PULLDOWN_DISABLE;
    gpio_set_direction(gpio_num, GPIO_MODE_OUTPUT);
}

/**
 * @brief 设置GPIO引脚的高低电平。
 * @param gpio_num 要控制的GPIO编号。
 * @param level 要设置的电平 (0=低电平, 1=高电平)。
 */
static void gpio_set_level(gpio_num_t gpio_num, int level)
{
    gpio_set_level(gpio_num, level);
}

/**
 * @brief 主应用入口函数。
 * @param arg 无参数。
 */
void app_main(void)
{
    ESP_LOGI(TAG, "Application started on ESP32-S3");

    // 假设我们使用GPIO 45作为LED输出 (根据环境信息，这里用一个常见的示例引脚进行演示，实际需根据硬件连接调整)
    // 注意：GPIO 45在ESP32-S3上可能被其他功能占用，请根据您的具体板卡接线核实。
    const gpio_num_t led_gpio = 45; // 使用示例引脚

    ESP_LOGI(TAG, "Configuring GPIO %d as output.", led_gpio);
    gpio_init(led_gpio);

    // 初始化时，将LED设置为低电平（假设是按键或外部驱动的默认状态）
    gpio_set_level(led_gpio, 0);

    while (1) {
        // 点亮LED (设置为高电平)
        ESP_LOGI(TAG, "Setting LED to HIGH");
        gpio_set_level(led_gpio, 1);
        vTaskDelay(pdMS_TO_TICKS(500)); // 延迟500毫秒

        // 熄灭LED (设置为低电平)
        ESP_LOGI(TAG, "Setting LED to LOW");
        gpio_set_level(led_gpio, 0);
        vTaskDelay(pdMS_TO_TICKS(500)); // 延迟500毫秒
    }
}