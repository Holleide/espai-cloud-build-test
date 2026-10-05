#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"

static const char *TAG = "main";

/**
 * @brief 主应用入口函数，实现 LED 点灯逻辑。
 *
 * 该函数初始化日志系统并启动一个无限循环任务来控制 LED 闪烁。
 *
 * @param () 无参数
 */
void app_main(void) {
    // 初始化日志系统
    ESP_LOGI(TAG, "Application starting...");

    // 创建一个任务用于控制 LED 闪烁，避免阻塞主循环
    TaskHandle_t led_task_handle;
    const char *led_tag = "led_control";

    // 创建任务并指定优先级（可根据需要调整）
    esp_err_t err = xTaskCreate(
        led_control_task,       // 要执行的任务函数
        "led_control",          // 任务名称
        4096,                   // 堆栈大小 (字节)
        NULL,                   // 参数 (这里不需要传递额外参数)
        5,                      // 优先级
        &led_task_handle         // 任务句柄
    );

    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Failed to create LED control task: %d", err);
        // 如果创建任务失败，可以考虑进入错误处理流程
        return;
    }

    ESP_LOGI(TAG, "LED control task created successfully.");

    // 等待任务完成（在实际应用中这部分通常是无限循环或等待事件）
    // 保持主线程运行，让任务在后台执行
    while (1) {
        vTaskDelay(pdMS_TO_TICKS(5000)); // 主循环等待 5 秒后继续
    }

    /* LED 控制任务函数 */
    void led_control_task(void *pvParameters) {
        // 定义 LED 引脚（根据 esp32s3 的实际连接，这里使用一个常见的 GPIO 作为示例）
        // **注意：请根据您的硬件实际引脚修改此处**
        const int led_pin = 45; // 假设 GPIO45 用于点灯 (需要核对实际硬件连接)
        const char *led_tag = "led_control";

        ESP_LOGI(led_tag, "LED control task started.");

        while (1) {
            // 设置 LED 为高电平（点亮）
            // 这里的 GPIO 操作需要使用 esp_driver_gpio 组件的 API，此处仅作逻辑演示
            // 在实际项目中，请确保已正确包含并使用了相应的驱动 API。
            // 例如：gpio_set_level(led_pin, 1);

            ESP_LOGI(led_tag, "LED ON");
            vTaskDelay(pdMS_TO_TICKS(2000)); // 点亮 2 秒

            // 设置 LED 为低电平（熄灭）
            // 例如：gpio_set_level(led_pin, 0);
            ESP_LOGI(led_tag, "LED OFF");
            vTaskDelay(pdMS_TO_TICKS(3000)); // 熄灭 3 秒
        }
    }
}