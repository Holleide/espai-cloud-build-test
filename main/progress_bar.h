#ifndef PROGRESS_BAR_H
#define PROGRESS_BAR_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief 初始化 LEDC PWM 进度条系统
 * 
 * 配置 LEDC 为 20Hz 刷新率，使用 GPIO13（LED）作为 PWM 输出通道。
 * 注意：ESP32-S3 默认使能了 Octal PSRAM，需禁用以避免占用 GPIO35-37。
 */
void progress_bar_init(void);

/**
 * @brief 更新进度条状态（0~100%）
 * 
 * @param progress 当前百分比（0-100）
 */
void progress_bar_update(int progress);

/**
 * @brief 停止进度条
 */
void progress_bar_stop(void);

#ifdef __cplusplus
}
#endif

#endif /* PROGRESS_BAR_H */