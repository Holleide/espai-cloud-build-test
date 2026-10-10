// File: main/led.c
//
// ESP32-S3 点灯驱动：led_init() 配置 GPIO 为推挽输出，led_task() 以 500ms 周期翻转电平。
// 每次电平变化都通过串口打印 IO CHANGE 日志，便于在 QEMU 仿真输出里直接观察 GPIO 变化
// （QEMU 串口采集只有日志画面，GPIO 无波形显示，故以日志形式呈现 IO 状态）。
// 修复历史（2026-10-11 云端编译报错）：
//   - 旧版 gpio_config_t 用了不存在的 in_type / driver_mode 字段与 GPIO_INPUT 常量
//     （ESP-IDF 正确字段为 mode / pin_bit_mask / intr_type）；
//   - 旧版在 led.c 与 main.c 各定义了一个 app_main → 链接期重复符号；
//   - 旧版任务栈仅 4 字节，创建成功也会立即栈溢出崩溃。
#include "driver/gpio.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

/* 板载用户 LED 引脚：ESP32-S3 常见为 GPIO2（若为 WS2812 灯珠板请改为 48 并换驱动） */
#define LED_PIN 2

static const char *TAG = "LED";

void led_init(void) {
    gpio_config_t cfg = {
        .pin_bit_mask = 1ULL << LED_PIN,          // 目标引脚
        .mode = GPIO_MODE_OUTPUT,                 // 推挽输出
        .pull_up_en = GPIO_PULLUP_DISABLE,        // 输出脚无需上拉
        .pull_down_en = GPIO_PULLDOWN_DISABLE,    // 输出脚无需下拉
        .intr_type = GPIO_INTR_DISABLE,           // 不使用中断
    };
    gpio_config(&cfg);
    gpio_set_level(LED_PIN, 0);                   // 初始熄灭
    ESP_LOGI(TAG, "IO INIT: GPIO%d = OUTPUT, 初始电平=%d (LED OFF)",
             LED_PIN, gpio_get_level(LED_PIN));
}

void led_task(void *pvParameters) {
    (void)pvParameters;
    int level = 0;
    for (;;) {
        level = !level;
        gpio_set_level(LED_PIN, level);           // 翻转 LED：亮→灭 / 灭→亮
        /* 每次翻转打印 IO 变化（QEMU 串口输出即仿真“画面”） */
        ESP_LOGI(TAG, "IO CHANGE: GPIO%d -> %d (%s)",
                 LED_PIN, level, level ? "LED ON" : "LED OFF");
        vTaskDelay(pdMS_TO_TICKS(500));           // 500ms 周期
    }
}
