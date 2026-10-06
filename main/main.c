#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "progress_bar.h"

static const char *TAG = "main";

void app_main(void) {
    ESP_LOGI(TAG, "ESP-AI Studio project started");

    // 初始化进度条模块（LED 指示灯 PWM）
    progress_bar_init();

    int progress = 50;
    while (1) {
        // 更新进度条状态
        progress_bar_update(progress);

        // 模拟进度变化（每 2 秒增加）
        if (++progress >= 100) {
            progress = 0;
        }
        ESP_LOGD(TAG, "Progress: %d%%", progress);

        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}
