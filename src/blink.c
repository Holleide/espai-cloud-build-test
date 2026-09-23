/**
 * @file blink.c
 * @brief Blink control logic for ESP32-C3
 * 
 * This module provides the core functionality for controlling the LED blink pattern.
 * Uses esp_timer for hardware-backed precise timing to avoid OS scheduling drift.
 */

#include "bsp/led.h"
#include "timer.h"

/**
 * @brief Toggle LED on/off and start blinking timer
 * @param led_state Current state of the LED (true = ON, false = OFF)
 */
void blink_toggle(esp_led_state_t led_state) {
    // Toggle LED to opposite state
    esp_led_set_state(!led_state);

    // Start or restart the blinking timer based on current state
    if (led_state) {
        // LED is ON - start timer for OFF period
        timer_start(TIMER_OFF, BLINK_INTERVAL_MS);
    } else {
        // LED is OFF - start timer for ON period
        timer_start(TIMER_ON, BLINK_INTERVAL_MS);
    }
}

/**
 * @brief Start blinking sequence (LED starts in ON state)
 */
void blink_init() {
    // Initialize LED to ON state and start the first cycle
    esp_led_set_state(true);  // LED ON
    timer_start(TIMER_OFF, BLINK_INTERVAL_MS);
}
