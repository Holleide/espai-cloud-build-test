/**
 * @file button.h
 * @brief 按钮输入模块接口定义
 * 
 * 功能：检测按键状态，支持轮询模式和中断模式
 */

#ifndef BUTTON_H
#define BUTTON_H

#ifdef __cplusplus
extern "C" {
#endif

void button_init(int pin, int is_active_high);
int button_read(void);
void button_toggle(void);
bool button_is_pressed(void);

#ifdef __cplusplus
}
#endif

#endif // BUTTON_H