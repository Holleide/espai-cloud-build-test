---

## Common Pitfalls on ESP32-S3

| Symptom | Cause / Fix |
|---|---|
| `fatal error: driver/gpio.h: No such file` | Missing `REQUIRES driver` in `main/CMakeLists.txt` |
| LED doesn't light, no log output | Wrong UART port — ESP32-S3 has **two** USB ports; the one labeled **UART** (not **USB**) is the console |
| `flash read err, 0x1000` | On ESP32-S3 the bootloader is at **0x0**, not 0x1000. Re-flash with `idf.py -p PORT flash` (no manual offset needed) |
| `undefined reference to gpio_set_level` | Forgot `REQUIRES driver` |
| WS2812 stays dark | GPIO 48 needs `led_strip` (RMT peripheral), not raw GPIO toggle |

---

## Expected Monitor Output