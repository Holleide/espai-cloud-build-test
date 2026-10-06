/**
 * @file progress_bar.c
 * @brief 进度条 UI 模块实现（ESP-IDF v6.0.1 + ESP32-S3）
 *
 * 使用 LEDC PWM 调光显示百分比，GPIO 状态指示灯反馈。优先用带 DMA 的 LEDC 外设而非软件轮询。
 */

#include <stdint.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "driver/ledc.h"
#include "progress_bar.h"

#define TAG "PROGRESS_BAR"

/* 内部状态：避免多线程竞争（本模块单任务设计）*/
typedef struct {
    int ledc_channel;
    uint32_t ledc_pin;
    int status_gpio;
    progress_bar_update_cb callback;
    volatile int percent;      /* 当前百分比，volatile 供回调安全读取 */
    volatile int status;       /* 状态码 */
} progress_bar_state_t;

static progress_bar_state_t g_progress_state = {0};

/* LEDC PWM 分辨率表（用于 duty 计算）*/
const uint32_t ledc_resolutions[] = {
    LEDC_RESOLUTION_8BIT,   /* 4096 */
    LEDC_RESOLUTION_10BIT,  /* 1024 */
    LEDC_RESOLUTION_12BIT   /* 4096 */
};

/* LEDC 模式配置（PWM）*/
typedef struct {
    uint32_t speed_hz;      /* PWM 频率：250Hz-8kHz 对应不同分辨率 */
    uint32_t resolution;
} ledc_mode_config_t;

static ledc_mode_config_t g_ledc_mode = {250, LEDC_RESOLUTION_10BIT};

/**
 * @brief 获取 PWM duty（占空比）：0-4095（对应 0%-100%）*/
static uint32_t get_pwm_duty(int percent) {
    return (uint32_t)((percent * g_ledc_mode.speed_hz) / 1000.0);
}

/**
 * @brief 初始化 LEDC PWM 和 GPIO 状态灯*/
void progress_bar_init(int ledc_channel, int ledc_pin, int status_gpio, progress_bar_update_cb callback) {
    /* 检查参数合法性（MUST：参数校验）*/
    if (ledc_channel < 0 || ledc_channel > 3) return;

    /* 1. 初始化 GPIO: LEDC PWM 输出引脚 + 状态灯 */
    gpio_config_t io_conf = {0};
    io_conf.intr_type = GPIO_INTR_DISABLE;
    io_conf.mode = GPIO_MODE_OUTPUT;           /* 配置为输出模式 */
    io_conf.pull_up_en = 0;                    /* 禁用内部上拉（避免干扰 LEDC）*/
    io_conf.pull_down_en = 0;
    io_conf.io_num = ledc_pin;
    gpio_config(&io_conf);

    status_gpio = (status_gpio >= -1 && status_gpio < GPIO_NUM_MAX) ? status_gpio : -1;

    /* 2. LEDC 初始化（带 DMA，避免阻塞）*/
    if (g_ledc_mode.speed_hz > LEDC_HIGH_SPEED_MODE_MAX_HZ) {
        g_ledc_mode.speed_hz = LEDC_HIGH_SPEED_MODE_MAX_HZ;   /* 限速 */
    }

    ledc_timer_config_t timer_conf = {0};
    timer_conf.timer_user_bitwidth = (g_ledc_mode.resolution == LEDC_RESOLUTION_12BIT) ? 12 : g_ledc_mode.resolution;
    timer_conf.duty_resolution = timer_conf.timer_user_bitwidth;   /* 分辨率与 duty 一致 */
    timer_conf.freq_hz = g_ledc_mode.speed_hz;
    timer_conf.clk_cfg = LEDC_CLK_AUTO;
    timer_conf.timer_sel = ledc_channel;

    if (ledc_timer_config(&timer_conf) != ESP_OK) {
        /* LEDC 初始化失败，设置错误状态 */
        g_progress_state.status = -1;           /* 非零表示错误 */
        return;
    }

    /* LEDC PWM 配置（通道）*/
    ledc_channel_config_t channel_conf = {0};
    channel_conf.intr_type = GPIO_INTR_DISABLE;
    channel_conf.mode = LEDC_MODE_PWM;          /* PWM 模式 */
    channel_conf.duty = LEDC_DUTY_50_PCT;       /* 初始占空比 50%（50% 亮度）*/
    channel_conf.intr_ena = 0;
    channel_conf.timer_sel = ledc_channel;      /* 关联到 timer */

    if (ledc_channel_config(&channel_conf) != ESP_OK) {
        g_progress_state.status = -1;
        return;
    }

    /* 3. GPIO 状态灯初始化（拉低，表示未激活）*/
    gpio_set_level(status_gpio, 0);

    /* 4. 存储配置，供回调安全读取 */
    g_progress_state.ledc_channel = ledc_channel;
    g_progress_state.ledc_pin = (uint32_t)ledc_pin;
    g_progress_state.status_gpio = status_gpio;
    g_progress_state.callback = callback;
    g_progress_state.percent = 0;
    g_progress_state.status = 0;                /* 初始正常 */

    ESP_LOGI(TAG, "Progress bar initialized: LEDC ch%d pin%d status gpio%d", ledc_channel, ledc_pin, status_gpio);
}

/**
 * @brief 更新进度条显示（非阻塞，使用 LEDC PWM）*/
void progress_bar_update(int percent, int status) {
    /* MUST：参数校验 */
    if (percent < 0 || percent > 100 || status != 0 && status != -1) return;

    g_progress_state.percent = percent;        /* 写状态（volatile）*/

    /* 计算 PWM duty（对应亮度百分比）：duty 范围 0-4095（LEDC_RESOLUTION_12BIT）*/
    uint32_t duty = (uint32_t)((percent * g_ledc_mode.speed_hz) / 1000.0);

    /* LEDC PWM 写 duty（非阻塞，带 DMA）：不阻塞主循环 */
    ledc_write_gpio(g_progress_state.ledc_pin, duty);

    /* 状态灯更新：status=0 → 正常（亮），status=-1 → 错误（灭）*/
    if (g_progress_state.status == -1) {
        gpio_set_level(g_progress_state.status_gpio, 0);
    } else {
        gpio_set_level(g_progress_state.status_gpio, percent > 0 ? 1 : 0);
    }

    /* 回调通知：由应用层自行决定是否阻塞（如 UI 刷新）*/
    if (g_progress_state.callback) {
        g_progress_state.callback(percent);
    }
}

/**
 * @brief 关闭进度条显示（停止 PWM + LEDC = 亮度归零，状态灯灭）*/
void progress_bar_stop(void) {
    /* LEDC PWM 写 duty=0（亮度归零）*/
    ledc_write_gpio(g_progress_state.ledc_pin, 0);

    /* GPIO 状态灯关闭 */
    if (g_progress_state.status_gpio >= -1 && g_progress_state.status_gpio < GPIO_NUM_MAX) {
        gpio_set_level(g_progress_state.status_gpio, 0);
    }

    ESP_LOGI(TAG, "Progress bar stopped.");
}
