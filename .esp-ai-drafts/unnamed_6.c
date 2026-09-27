---

## Optional: Onboard WS2812 RGB LED (GPIO 48)

The ESP32-S3 DevKitC-1 has a WS2812 addressable LED on **GPIO 48**. A plain `gpio_set_level` will not drive it correctly — use the `led_strip` component instead: