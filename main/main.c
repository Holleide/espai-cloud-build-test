/*
 * main.c - ESP-IDF v6.0.1 + esp32s3 (ESP32-S3)
 * 功能：LED 闪烁演示 + 按键消抖状态机
 */
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"

static const char *TAG = "main";

/* LED 控制函数（保留原逻辑，未修改） */
void led_flash_start(void) {
    gpio_init(12);
    gpio_set_direction(12, GPIO_MODE_OUT);
    gpio_set_level(12, 0);
}

void led_flash_stop(void) {
    gpio_set_level(12, 1);
}

/* LED 闪烁任务函数（保留原逻辑，未修改） */
static void vLedFlashTask(void *pvParameters) {
    int flash_period_ms = (int)(pdMS_TO_TICKS(500));
    while (1) {
        led_flash_start();
        vTaskDelay(pdMS_TO_TICKS(flash_period_ms));
        led_flash_stop();
        vTaskDelay(pdMS_TO_TICKS(flash_period_ms));
    }
}

/* 按键消抖状态机实现（非阻塞版） */
static bool button_debounce_state_machine(uint8_t key_pin, bool debounce_enabled) {
    static uint32_t last_valid_time = 0;
    static int last_button_state = -1;
    const int debounce_interval_ms = 15;

    ESP_LOGI(TAG, "Button debounce state machine initialized");

    while (true) {
        uint32_t current_time = esp_timer_get_time();
        uint32_t elapsed_ms = (current_time - last_valid_time) / 1000;

        int button_state = gpio_level_get(key_pin);

        /* 消抖：需要间隔 debounce_interval_ms 毫秒后再次检测到相同电平才算有效 */
        if (elapsed_ms >= debounce_interval_ms && last_button_state != -1) {
            ESP_LOGD(TAG, "Button state changed: %s", button_state ? "HIGH" : "LOW");
            /* 消抖完成，记录有效状态并重置计时器 */
            last_valid_time = current_time;
        }

        return debounce_enabled && (button_state != last_button_state);
    }
}
