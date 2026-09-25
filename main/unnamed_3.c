#ifndef CODTEST_H
#define CODTEST_H

#include "driver/gpio.h"

// LED 引脚定义（GPIO2，对应 pin 19）
#define LED_PIN GPIO_NUM_2

// 函数声明
void codetest_init(void);      // 初始化（返回 esp_err_t）
void codetest_loop(void);      // 循环执行闪烁（单次调用 + vTaskDelay）

#endif // CODTEST_H