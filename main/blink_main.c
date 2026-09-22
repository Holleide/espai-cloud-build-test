#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "esp_log.h"

static const char *TAG = "blink";

#define BLINK_GPIO 2

void app_main(void)
{
    ESP_LOGI(TAG, "LED 闪烁测试开始");
    gpio_reset_pin(BLINK_GPIO);
    gpio_set_direction(BLINK_GPIO, GPIO_MODE_OUTPUT);

    int led_state = 0;
    while (1) {
        gpio_set_level(BLINK_GPIO, led_state);
        ESP_LOGI(TAG, "LED %s", led_state ? "ON" : "OFF");
        led_state = !led_state;
        vTaskDelay(pdMS_TO_TICKS(500));
    }
}
