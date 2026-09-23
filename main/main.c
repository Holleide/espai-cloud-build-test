#include "main.h"
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

#define ESP32_GPIO_BUTTON_PIN 4
#define ESP32_GPIO_LED_PIN 2

static void led_blink_task(void *pvParameters) {
    for (;;) {
        // toggle LED on
        gpio_set_level(ESP32_GPIO_LED_PIN, 1);
        
        vTaskDelay(pdMS_TO_TICKS(500));

        // turn off LED
        gpio_set_level(ESP32_GPIO_LED_PIN, 0);
        
        vTaskDelay(pdMS_TO_TICKS(500));
    }
}

static void app_main(void) {
    // Initialize button on GPIO4 with pull-up
    gpio_init(ESP32_GPIO_BUTTON_PIN);
    gpio_set_pull_mode(ESP32_GPIO_BUTTON_PIN, GPIO_PULLUP_ONLY);
    
    // Initialize LED on GPIO2 as output
    gpio_init(ESP32_GPIO_LED_PIN);
    gpio_set_dir(ESP32_GPIO_LED_PIN, GPIO_OUT);

    xTaskCreatePinnedToCore(le_blink_task, "BlinkTask", 1024, NULL, 1, NULL, 1);
}