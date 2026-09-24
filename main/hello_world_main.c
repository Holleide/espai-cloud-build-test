#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"

static const char *TAG = "hello_world";

void app_main(void)
{
    ESP_LOGI(TAG, "ESP-IDF Hello World!");
    ESP_LOGI(TAG, "编译环境测试通过！");
    ESP_LOGI(TAG, "Free tick port: %lu", (unsigned long)xPortGetCoreID());

    // 故意引入一个未定义函数调用，制造真实的编译错误（用于测试编译失败自动回喂 AI）
    force_compile_error_undefined();

    int count = 0;
    while (1) {
        ESP_LOGI(TAG, "count: %d", count++);
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}
