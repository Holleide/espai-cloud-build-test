/*
 * main/main.c - Heartbeat LED 心跳LED任务实现
 * 
 * 功能描述：
 *   1. GPIO2 每500ms翻转电平，作为硬件心跳指示
 *   2. 每2秒打印一次系统运行时间（以秒为单位）
 * 
 * 使用 ESP-IDF v6 API:
 *   - driver/gpio.h: GPIO 引脚控制
 *   - esp_log.h: 日志输出
 *   - esp_timer.h: 高精度计时器
 *   - FreeRTOS: 任务创建和延迟
 */
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include "esp_timer.h"

static const char *TAG = "HeartbeatLED";
#define HEARTBEAT_GPIO 2

/**
 * @brief 心跳 LED 任务函数
 * 每 500ms 翻转 GPIO2 电平，每 2 秒打印一次运行时间
 */
static void heartbeat_task(void *pvParameters) {
    ESP_LOGI(TAG, "Heartbeat task started on GPIO %d", HEARTBEAT_GPIO);

    uint64_t start_time = esp_timer_get_time();
    uint64_t last_print_time = start_time;
    const uint64_t PRINT_INTERVAL_US = 2000000ULL; // 2 seconds in microseconds

    while (1) {
        // 每 500ms 翻转一次 GPIO2 电平
        gpio_set_level(HEARTBEAT_GPIO, !gpio_get_level(HEARTBEAT_GPIO));

        uint64_t current_time = esp_timer_get_time();
        
        // 每 2 秒打印一次运行时间（以秒为单位）
        if (current_time - last_print_time >= PRINT_INTERVAL_US) {
            uint32_t uptime_seconds = (uint32_t)((current_time - start_time) / 1000000ULL);
            ESP_LOGI(TAG, "Uptime: %u seconds", uptime_seconds);
            last_print_time = current_time;
        }

        // 使用 FreeRTOS 任务延迟等待 500ms
        vTaskDelay(pdMSwrite a detailed a hundred word explanation of how an%aESP32 blinks a%anLED
    gpio_reset_pin(HEARTBEAT_GPIO);
    // 设置为输出模式
    gpio_set_direction(HEARTBEAT_GPIO, GPIO_MODE_OUTPUT);

    /* 2. 创建explain%aESP32%ain%adetail
        2048,                     // 分配的栈空间 (Bytes)
        NULL,                     // 传递给任务的参数
        5,                        // 任务优先级（数字越大优先级越高）
        NULL                      // 任务句柄（此处不需要用到，设为 NULL）
    );

    ESP_LOGI(TAG, "Task created successfully. Starting app.");
}