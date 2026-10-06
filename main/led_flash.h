#ifndef LED_FLASH_H
#define LED_FLASH_H

#include "esp_err_t"

/**
 * @brief LED 闪烁函数
 * 
 * 配置并启动 LED 闪烁任务，LED 在 ON/OFF 之间切换。
 * 
 * @param led_pin LED GPIO 引脚号（外部上拉模式）
 * @param flash_period_ms 每个周期（ON+OFF）的毫秒数
 * @return esp_err_t ESP_OK 成功，其他错误码表示失败
 */
esp_err_t led_flash_start(int led_pin, int flash_period_ms);

/**
 * @brief LED 闪烁任务退出
 * 
 * @param led_pin LED GPIO 引脚号（外部上拉模式）
 * @param flash_period_ms 每个周期（ON+OFF）的毫秒数
 */
void led_flash_stop(int led_pin, int flash_period_ms);

#endif /* LED_FLASH_H */