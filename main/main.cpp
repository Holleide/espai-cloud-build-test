/**
 * @file main.cpp
 * @brief ESP32-C3 Blink Demo using esp_timer for precise timing
 * 
 * This demo toggles an LED connected to GPIO5 at a user-defined interval.
 * Uses ESP-IDF's esp_timer API (hardware-backed) instead of software delays
 * for accurate, load-independent blink intervals.
 */

#include <esp_log.h>
#include <driver/gpio.h>
#include <esp_timer.h>

// Include project configuration
#include "../include/config.h"

/** Tag for logging messages */
static const char* TAG = "blink_demo";

/** LED pin number (ESP32-C3 DevKit V1: GPIO5) */
static const gpio_num_t LED_PIN = LED_PIN;

/** Blink timer handle */
static esp_timer_handle_t blink_timer = NULL;

/** Timer callback that toggles the LED and reschedules itself */
static void timer_callback(void* arg) {
    ESP_LOGI(TAG, "Blink timer tick: tgl %d", esp_timer_get_time());

    // Toggle GPIO state (0=OFF, 1=ON for internal pull-down resistor LED)
    gpio_level_set(LED_PIN, 1);  // Turn LED ON

    // Schedule next blink after configured interval
    ESP_ERROR_CHECK(esp_timer_start_periodic(blink_timer, BLINK_INTERVAL_MS));
}

/** App main entry point */
void app_main(void) {
    ESP_LOGI(TAG, "ESP32-C3 Blink Demo starting...");

    /**@{**/ GPIO Initialization ***/
    
    // Enable GPIO peripheral clock and set pin to output with pull-down resistor
    gpio_set_direction(LED_PIN, GPIO_OUTPUT);
    gpio_reset_pullup(LED_PIN);   // Enable internal pull-down (LED is active low)
    
    ESP_LOGI(TAG, "GPIO initialized: LED pin = %d", LED_PIN);

    /**@}* ***/

    /**@{**/ Timer Initialization ***/
    
    // Create hardware-backed timer using esp_timer API
    // This provides precise sub-microsecond timing (vs. software delays)
    ESP_ERROR_CHECK(esp_timer_create(EspTimerType_DEFAULT, "blink_timer", 
                                      &blink_timer) != ESP_OK);

    ESP_LOGI(TAG, "Blink timer created: %p", blink_timer);

    /**@}* ***/

    /**@{**/ Timer Scheduling ***/
    
    // Start the periodic timer at configured interval
    if (blink_timer == NULL) {
        ESP_LOGE(TAG, "Failed to create blink timer");
        return;
    }

    ESP_ERROR_CHECK(esp_timer_start_periodic(blink_timer, BLINK_INTERVAL_MS));

    /**@}* ***/

    /**@{**/ Logging Initialization ***/
    
    // ESP-IDF automatically initializes logging on first ESP_LOG* call
    // No manual setup needed - will auto-initialize via timer_callback above
    
    /**@}* ***/

    ESP_LOGI(TAG, "Blink demo completed. LED should be blinking at %dms intervals.\n", 
             BLINK_INTERVAL_MS);
}
