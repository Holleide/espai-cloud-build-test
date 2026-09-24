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

    int count = 0;
    while (1) {
        ESP_LOGI(TAG, "count: %d", count++);
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}
