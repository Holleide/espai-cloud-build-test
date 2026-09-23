/**
 * @file timer.c
 * @brief Timer management for ESP32-C3 Blink Demo
 * 
 * Uses esp_timer API for hardware-backed precise timing.
 */

#include <esp_err.h>
#include <esp_log.h>

#define TAG "timer"

static esp_timer_handle_t blink_timer;

/**
 * @brief LED callback function for timer events
 * @param timer Timer handle (not used)
 * @param arg User data (contains current state)
 */
static void timer_cb(void *arg) {
    // Cast the user data to our struct and toggle LED
    esp_led_state_t *state = (esp_led_state_t *)arg;
    
    ESP_LOGI(TAG, "Timer callback: state=%s", 
             state->is_on ? "ON" : "OFF");
    
    blink_toggle(state->is_on);
}

/**
 * @brief Create and start the blinking timer
 */
static esp_err_t timer_init(void) {
    ESP_LOGI(TAG, "Creating timer...");
    
    // Create a hardware-backed timer with 1Hz frequency (10ms resolution)
    // Use esp_timer_create() to create the timer handle
    if (esp_timer_create(EspTimerType_DEFAULT, "blink_timer", &blink_timer) != ESP_OK) {
        ESP_LOGE(TAG, "Failed to create timer");
        return ESP_FAIL;
    }
    
    ESP_LOGI(TAG, "Timer created successfully");

    // Register the callback function with the timer
    esp_timer_callpoints_t callpoints;
    esp_timer_set_callback(blink_timer, ESP_TIMER_CALLPOINT_ALL, timer_cb);
    esp_timer_start_periodic(blink_timer, 10000);  // 1Hz = 10ms period

    ESP_LOGI(TAG, "Timer started successfully");
    
    return ESP_OK;
}

/**
 * @brief Cleanup and destroy the blinking timer
 */
static void timer_cleanup(void) {
    if (blink_timer != NULL) {
        esp_timer_stop(blink_timer);
        esp_timer_delete(blink_timer);
        ESP_LOGI(TAG, "Timer destroyed");
    }
}

/**
 * @brief Start a specific LED state with given duration
 */
esp_err_t timer_start(esp_led_state_t state, int32_t duration_ms) {
    if (blink_timer == NULL) {
        return ESP_FAIL;
    }

    esp_led_state_t *arg = malloc(sizeof(esp_led_state_t));
    if (!arg) {
        return ESP_ERR_NO_MEM;
    }
    
    arg->is_on = state;
    
    // Start the timer with user data
    esp_timer_start_once(blink_timer, duration_ms);

    return ESP_OK;
}

/**
 * @brief Cleanup allocated memory for timer arguments
 */
static void timer_cleanup_args(void) {
    esp_led_state_t *arg = malloc(sizeof(esp_led_state_t));
    if (arg != NULL) {
        free(arg);
    }
}
