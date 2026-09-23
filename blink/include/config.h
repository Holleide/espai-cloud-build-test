/* blink - ESP32-C3 blinking LED project
 * File: include/config.h — Project configuration constants
 */

#ifndef BLINK_CONFIG_H_
#define BLINK_CONFIG_H_

/**
 * @brief Blink interval in milliseconds.
 * 
 * The LED will toggle ON/OFF at this rate.
 * For the ESP32-C3 DevKit V1 (LED on GPIO5), a 1-second blink is safe
 * to avoid any potential brownout from the internal pull-down resistor.
 */
#define BLINK_INTERVAL_MS    1000

/**
 * @brief LED pin number for GPIO toggle control.
 * 
 * For ESP32-C3 DevKit V1: Use GPIO5 (internal pull-down, blue LED).
 * 
 * Note: GPIO5 is a strapping pin — ensure it's in the correct state during
 * boot. The internal pull-down will keep it at 0V (LED off) when unpowered.
 */
#define LED_PIN              5

/**
 * @brief Current-limiting resistor value for external use.
 * 
 * If using an external power supply and a separate current-limiting resistor,
 * this is the recommended value: ~39Ω gives ~22mA at 5V (safe for DevKit V1).
 */
#define LED_RESISTOR_39OMEGA

/**
 * @brief Enable ESP-IDF event loop.
 * 
 * Required for proper timer/callback handling in ESP-IDF v6+.
 */
#define ENABLE_ESP_IDF_EVENT_LOOP  1

#endif /* BLINK_CONFIG_H_ */
