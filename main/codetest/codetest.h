// File: main/codetest/codetest.h
// codetest 模块：使用 ESP-IDF GPIO 驱动翻转板载 LED（GPIO2）
// 注意：GPIO2 在部分芯片（如 ESP32-C3 / 经典 ESP32）上是 strapping 引脚，
//       上电瞬间的电平会影响启动模式；若烧录/启动异常请改用其它空闲引脚。

#ifndef CODETEST_H
#define CODETEST_H

#include <stdbool.h>
#include "esp_err.h"

#ifdef __cplusplus
extern "C" {
#endif

/// LED 所接的 GPIO 编号
#define CODETEST_LED_GPIO_NUM 2

/// LED 翻转间隔（毫秒）
#define CODETEST_BLINK_INTERVAL_MS 500

/**
 * @brief 初始化 codetest 模块：把 LED 引脚配置为推挽输出并点亮灭初始状态。
 *
 * @return ESP_OK 表示成功；其它值为具体的错误码（不会 panic，由调用方决定处理方式）
 */
esp_err_t codetest_init(void);

/**
 * @brief 执行一次循环体：翻转 LED 电平并延时。
 *
 * 本函数不阻塞死循环，需在调用方（app_main）里持续循环调用，
 * 以实现“永远运行”的效果。
 */
void codetest_loop(void);

/**
 * @brief 查询当前 LED 的逻辑状态。
 *
 * @return true 表示点亮，false 表示熄灭
 */
bool codetest_get_state(void);

#ifdef __cplusplus
}
#endif

#endif // CODETEST_H
