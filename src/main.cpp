// File: src/main.cpp
#include <Arduino.h>

// 定义LED连接的GPIO口和引脚（示例，需根据实际板卡调整）
#define LED_PIN 13 // STM32F103常用引脚，请核对您的开发板手册
#define LED_PORT GPIOC

/**
 * @brief Setup 函数：初始化系统。
 */
void setup() {
    // 初始化GPIO端口的时钟（需要根据STM32F103的实际时钟配置）
    // 假设已通过CubeMX或外部配置使能GPIOC的时钟
    // RCC_AHB1ENR |= RCC_AHB1ENR_GPIOCEN;

    // 配置LED引脚为输出模式 (使用HAL库函数)
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Pin = LED_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(LED_PORT, &GPIO_InitStruct);

    // 示例：点亮LED（假设是低电平有效，根据实际硬件调整）
    HAL_GPIO_WritePin(LED_PORT, LED_PIN, GPIO_PIN_RESET);
}

/**
 * @brief Loop 函数：主程序循环。
 */
void loop() {
    // 保持LED点亮
    HAL_Delay(1000);
}