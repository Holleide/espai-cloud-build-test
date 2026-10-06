/**
 * @file progress_bar.h
 * @brief 进度条 UI 模块（ESP-IDF v6.0.1 + ESP32-S3）
 *
 * 功能：LED LEDC PWM 调光显示进度百分比，配合 GPIO 状态指示灯。
 * 使用方式：在 app_main() 中调用 progress_bar_init(); 循环中调用 progress_bar_update(percent, status);
 */

#ifndef MAIN_PROGRESS_BAR_H
#define MAIN_PROGRESS_BAR_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/* 回调函数：进度更新完成时调用（可中断）*/
typedef void (*progress_bar_update_cb)(int percent);

/**
 * @brief 初始化进度条模块
 *
 * 参数:
 *   ledc_channel: LEDC 通道号 (0/1/2/3)
 *   ledc_pin: GPIO 编号（LED PWM 输出引脚）
 *   status_gpio: GPIO 编号（状态指示灯，高电平表示正常）
 *   callback: 进度更新完成回调
 */
void progress_bar_init(int ledc_channel, int ledc_pin, int status_gpio, progress_bar_update_cb callback);

/**
 * @brief 更新进度条显示
 *
 * 参数:
 *   percent: 0-100 的整型百分比
 *   status: 状态码，0=正常，非零=错误（LEDC/status GPIO 会反映）
 */
void progress_bar_update(int percent, int status);

/**
 * @brief 关闭进度条显示
 */
void progress_bar_stop(void);

#ifdef __cplusplus
}
#endif

#endif /* MAIN_PROGRESS_BAR_H */
