/*
 * ESP32-S3 Blink LED Example
 * Toggles an LED on GPIO 2 every 500 ms using FreeRTOS vTaskDelay.
 *
 * On the ESP32-S3 DevKitC-1, GPIO 2 is a free user pin.
 * The onboard WS2812 RGB LED is on GPIO 48 (see optional section below).
 */

#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "esp_log.h"

#define BLINK_GPIO GPIO_NUM_2
#define BLINK_INTERVAL_MS 500

static const char *TAG = "blink";

static void blink_task(void *arg)
{
    /* Configure GPIO as output */
    gpio_reset_pin(BLINK_GPIO);
    gpio_set_direction(BLINK_GPIO, GPIO_MODE_OUTPUT);

    int state = 0;
    while (1) {
        gpio_set_level(BLINK_GPIO, state);
        ESP_LOGI(TAG, "LED %s (GPIO %d)", state ? "ON " : "OFF", BLINK_GPIO);
        state = !state;
        vTaskDelay(pdMS_TO_TICKS(BLINK_INTERVAL_MS));
    }
}

void app_main(void)
{
    ESP_LOGI(TAG, "Starting blink task on ESP32-S3");
    xTaskCreate(blink_task, "blink_task", 4096, NULL, 5, NULL);
}