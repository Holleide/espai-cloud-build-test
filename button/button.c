/**
 * @file button.c
 * @brief 按钮输入模块实现
 */

#include "button.h"
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "esp_log.h"

static const char *TAG = "BUTTON";
static int button_pin = -1;
static bool is_active_high = false;

/**
 * @brief 初始化按钮 GPIO
 * @param pin 按钮引脚号（默认 GPIO4）
 * @param is_active_high 是否高电平有效（默认 true）
 */
void button_init(int pin, int is_active_high) {
    if (pin == -1) {
        ESP_LOGE(TAG, "无效引脚");
        return;
    }
    
    gpio_set_direction(pin, GPIO_MODE_INPUT);
    gpio_pullup_enable(pin);  // 上拉电阻，释放时为高电平
    
    button_pin = pin;
    this->is_active_high = is_active_high;
}

/**
 * @brief 读取按钮状态（1=按下，0=未按下）
 */
int button_read(void) {
    bool level = gpio_level_get(button_pin);
    
    if (this->is_active_high) {
        return (level == true) ? 1 : 0;
    } else {
        return (level == false) ? 1 : 0;
    }
}

/**
 * @brief 切换按钮状态（按下/释放）
 */
void button_toggle(void) {
    bool level = gpio_level_get(button_pin);
    
    if (this->is_active_high) {
        if (level == true) {
            gpio_clear_bit(button_pin);
        } else {
            gpio_set_level(button_pin, 1);
        }
    } else {
        if (level == false) {
            gpio_clear_bit(button_pin);
        } else {
            gpio_set_level(button_pin, 0);
        }
    }
}

/**
 * @brief 检查按钮是否按下（当前状态）
 */
bool button_is_pressed(void) {
    return button_read() == 1;
}