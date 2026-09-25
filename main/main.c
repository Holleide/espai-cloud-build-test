// File: main/main.c
// 工程入口：初始化 codetest 模块后，永久循环执行其循环体

#include "codetest.h"

#include "esp_err.h"
#include "esp_log.h"

// ESP-IDF v6 起需显式包含 FreeRTOS 头文件
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

static const char *TAG = "main";

void app_main(void)
{
    // 初始化失败属于不可恢复错误，直接 abort 以便串口定位问题
    ESP_ERROR_CHECK(codetest_init());

    ESP_LOGI(TAG, "进入主循环，持续翻转 GPIO%d 上的 LED", CODETEST_LED_GPIO_NUM);

    // 永远运行：每次调用执行一次“翻转 + 延时”
    while (1) {
        codetest_loop();
    }
}
