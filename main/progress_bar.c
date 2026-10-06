#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "driver/ledc.h"
#include "driver/timer.h"
#include "progress_bar.h"

#define LEDC_CHANNEL 1
#define LED_PIN 13
#define PWM_FREQ_HZ 50   // 20Hz = 50ms 刷新率（LED 肉眼可见）
#define LEDC_RES 8       // 占空比精度：(4096-1)/2^8 ≈ 1.5%

static ledc_timer_t ledc_timer;
static ledc_channel_t ledc_channel;

static esp_timer_handle_t progress_timer = NULL;

/**
 * LEDC 定时器回调，每 50ms 更新一次 PWM 占空比
 */
static void ledc_callback(void) {
    static int last_progress = -1;

    if (last_progress >= 0 && last_progress <= 100) {
        // 将百分比转换为 LEDC 占空比（0-4095）
        uint32_t duty_cycle = (uint32_t)(last_progress * 4096 / 100);

        ledcWrite(LEDC_CHANNEL, duty_cycle);
    } else {
        // 停止时关闭 LEDC
        if (duty_cycle > 0) {
            ledcWrite(LEDC_CHANNEL, 0);
        }
    }

    last_progress = progress_bar_update(last_progress);
}

/**
 * @brief 初始化进度条系统
 */
void progress_bar_init(void) {
    ESP_LOGI(TAG, "Progress bar init");

    // 禁用 Octal PSRAM（ESP32-S3 默认启用，会占用 GPIO35-37）
    ledc_timer_config_t timer_conf = {.duty_resolution = LEDC_RES,
                                       .timer_num = LEDC_TIMER_0};
    ledc_timer_init(&timer_conf);

    // 配置 PWM 通道（GPIO13）
    ledc_channel_config_t channel_conf = {
        .channel = LEDC_CHANNEL,
        .gpio_num = LED_PIN,
        .duty_resolution = LEDC_RES,
        .inverted = false
    };
    ledc_channel_config(&channel_conf);

    // 配置定时器回调（50ms）
    timer_config_t timer_params = {
        .timer_num = LEDC_TIMER_0,
        .div_num = 8,       // 24MHz / 8 = 3MHz，3MHz/50Hz = 60ms... 需要重新计算
        .trigger_buf_entries = 10,
        .counter_mode = TIMER_COUNT_UP,
        .auto_reload = true
    };

    esp_timer_create_with_args(&timer_params, ledc_callback, "progress_bar");

    ESP_LOGI(TAG, "Progress bar initialized successfully");
}

/**
 * @brief 更新进度条状态
 */
static void progress_update_internal(int progress) {
    if (progress < 0) progress = 0;
    if (progress > 100) progress = 100;

    uint32_t duty_cycle = (uint32_t)(progress * 4096 / 100);
    ledcWrite(LEDC_CHANNEL, duty_cycle);
}

/**
 * @brief 更新进度条（线程安全，可被定时器/主循环并发调用）
 */
void progress_bar_update(int progress) {
    if (progress >= 0 && progress <= 100) {
        // 使用临界区保护并发访问
        static int locked = 0;
        uint8_t lock;

        esp_lock(&lock);
        if (locked == 0) {
            last_progress = progress;
            ESP_LOGD(TAG, "Progress: %d%%", progress);
            esp_unlock(&lock);
        } else {
            ESP_LOGW(TAG, "Progress update skipped (busy)");
        }

        esp_unlock(&lock);
    }
}

/**
 * @brief 停止进度条
 */
void progress_bar_stop(void) {
    ledcWrite(LEDC_CHANNEL, 0);
    if (progress_timer != NULL) {
        esp_timer_delete(progress_timer);
        progress_timer = NULL;
    }
    ESP_LOGI(TAG, "Progress bar stopped");
}

/**
 * @brief 初始化进度条系统（封装版本）
 */
void progress_bar_init(void) {
    ESP_LOGI(TAG, "Progress bar init");

    // 禁用 Octal PSRAM（ESP32-S3 默认启用，会占用 GPIO35-37）
    ledc_timer_config_t timer_conf = {.duty_resolution = LEDC_RES,
                                       .timer_num = LEDC_TIMER_0};
    ledc_timer_init(&timer_conf);

    // 配置 PWM 通道（GPIO13）
    ledc_channel_config_t channel_conf = {
        .channel = LEDC_CHANNEL,
        .gpio_num = LED_PIN,
        .duty_resolution = LEDC_RES,
        .inverted = false
    };
    ledc_channel_config(&channel_conf);

    // 配置定时器回调：ESP32-S3 时钟源为 24MHz，目标 50Hz(20ms)
    // div_num = ceil(24_000_000 / (50 * 1000)) ≈ 48.96 → 取 49
    timer_config_t timer_params = {
        .timer_num = LEDC_TIMER_0,
        .div_num = 49,       // ESP32-S3@24MHz: 24MHz / (50Hz * 1000) ≈ 48.96 → ceil=49，实际周期≈(24e6)/(49*50)≈9.79ms
        .trigger_buf_entries = 10,
        .counter_mode = TIMER_COUNT_UP,
        .auto_reload = true
    };

    esp_timer_create_with_args(&timer_params, ledc_callback, "progress_bar");

    ESP_LOGI(TAG, "Progress bar initialized successfully");
}