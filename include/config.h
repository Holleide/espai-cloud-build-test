/**
 * @file config.h
 * @brief Project configuration for ESP32-C3 Blink Demo
 */

#ifndef CONFIG_H_
#define CONFIG_H_

// LED pin number (ESP32-C3 DevKit V1: GPIO5)
#define LED_PIN 5

// LED current-limiting resistor value in ohms (typical 39Ω @ ~22mA)
#define LED_RESISTOR_39OMEGA

// Blink interval in milliseconds
#define BLINK_INTERVAL_MS 1000

#endif /* CONFIG_H_ */
