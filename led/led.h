/**
 * @file led.h
 * @brief LED 控制模块接口定义
 * 
 * 功能：通过 GPIO 控制 LED 点亮/熄灭，支持闪烁、长亮等模式
 */

#ifndef LED_H
#define LED_H

#ifdef __cplusplus
extern "C" {
#endif

void led_init(void);
void led_blink(int pin, int frequency_ms);
void led_long_on(int pin);
void led_toggle(int pin);

#ifdef __cplusplus
}
#endif

#endif // LED_H