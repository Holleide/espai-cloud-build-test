// File: main/led.c
#include <string.h>
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

static const char *TAG = "LED";

#define LED_PIN 25        // ESP32-C3 GPIO25（RGB 灯的绿色通道，典型默认引脚）

void led_init(void) {
    gpio_config_t config = {.in_type = GPIO_INPUT,
                            .pull_up_en = true,
                            .pull_down_en = false,
                            .driver_mode = GPIO_MODE_OUTPUT};
    gpio_config(&config);  // 配置 GPIO25 为推挽输出，上拉电阻开启

    // 点亮 LED（高电平）
    gpio_set_level(LED_PIN, 1);
}

void led_task(void *pvParameters) {
    int32_t xNextDlTime;
    for (;;) {
        if (xTaskGetTickCount() >= xNextDlTime / portTICK_PERIOD_MS) {
            // 翻转 LED 状态：亮→灭/灭→亮
            gpio_set_level(LED_PIN, !gpio_get_level(LED_PIN));
            xNextDlTime = xTaskGetTickCount() + pdMS_TO_TICKS(500); // 0.5s 周期
        }
        vTaskDelay(pdMS_TO_TICKS(100));  // 预留时间，避免阻塞太久
    }
}

void app_main(void) {
    led_init();
    xTaskCreatePinnedToCore(led_task, TAG, 4, NULL, 3, NULL, 0);
}
