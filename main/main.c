/* ==============================================================================
 *  作        者：EspAiStudio · AI 代码工程师
 *  功 能 简 介：主程序入口源文件（工程：hhh）
 *               点灯 Demo - ESP-IDF GPIO 翻转 LED（驱动见 main/led.c）
 * ============================================================================== */

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

/* main/led.c 提供的点灯驱动 */
void led_init(void);
void led_task(void *pvParameters);

void app_main(void) {
    /* ========== 初始化 ========== */
    led_init();                                 // 配置 LED 引脚为输出并熄灭

    /* ========== 启动点灯任务（500ms 翻转一次） ========== */
    /* 栈单位为字节（ESP-IDF 约定），4096 字节足够承载 vTaskDelay 循环 */
    if (xTaskCreatePinnedToCore(led_task, "led_task", 4096, NULL, 3, NULL, 0) != pdPASS) {
        /* 任务创建失败则退化到当前任务内循环闪烁，保证点灯功能不丢失 */
        led_task(NULL);
    }
}
