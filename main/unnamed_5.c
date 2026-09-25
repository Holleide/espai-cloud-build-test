#include <stdio.h>
#include "esp_log.h"
#include "freertos/task.h"
#include "codetest.h"

static const char *TAG = "main";

void app_main(void) {
    // 初始化 LED（统一用 ESP_ERROR_CHECK 检查错误）
    esp_err_t ret = codetest_init();
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Failed to initialize LED: %d", ret);
        while(1) {
            vTaskDelay(pdMS_TO_TICKS(1000));
        }
    }

    // 进入主循环，驱动 codetest_loop()
    ESP_LOGI(TAG, "LED initialized. Start blinking...");
    while (1) {
        codetest_loop();      // 单次执行 + vTaskDelay
    }
}