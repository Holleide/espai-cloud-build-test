/**
 * @file led.c
 * @brief LED 控制模块实现
 */

#include "led.h"
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "esp_log.h"

static const char *TAG = "LED";
static volatile bool is_blinking = false;
static int current_pin = -1;

/**
 * @brief 初始化 LED GPIO（假设使用 GPIO5）
 */
void led_init(void) {
    ESP_LOGI(TAG, "初始化 LED (GPIO5)...");
    
    gpio_set_direction(GPIO_NUM_5, GPIO_MODE_OUTPUT);
    gpio_set_level(GPIO_NUM_5, 0);
}

/**
 * @brief LED 闪烁函数
 * @param pin LED 引脚号（默认 GPIO5）
 * @param frequency_ms 闪烁间隔（毫秒）
 */
void led_blink(int pin, int frequency_ms) {
    current_pin = pin;
    
    if (current_pin == -1) {
        ESP_LOGE(TAG, "无效引脚");
        return;
    }
    
    gpio_set_level(current_pin, 0);
    is_blinking = true;
    
    vTaskDelay(pdMS_TO_TICKS(frequency_ms));
    
    if (is_blinking) {
        gpio_set_level(current_pin, 1);
        ESP_LOGI(TAG, "LED 点亮");
    } else {
        gpio_clear_bit(current_pin);
        ESP_LOGI(TAG, "LED 熄灭");
    }
}

/**
 * @brief LED 长亮函数
 * @param pin LED 引脚号（默认 GPIO5）
 */
void led_long_on(int pin) {
    if (pin == -1) {
        ESP_LOGE(TAG, "无效引脚");
        return;
    }
    
    gpio_set_level(pin, 1);
}

/**
 * @brief LED 切换状态
 * @param pin LED 引脚号（默认 GPIO5）
 */
void led_toggle(int pin) {
    if (pin == -1) {
        ESP_LOGE(TAG, "无效引脚");
        return;
    }
    
    if (gpio_level_get(pin)) {
        gpio_clear_bit(pin);
    } else {
        gpio_set_level(pin, 1);
    }
}