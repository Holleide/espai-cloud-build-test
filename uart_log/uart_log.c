/**
 * @file uart_log.c
 * @brief UART 日志模块实现
 */

#include "uart_log.h"
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "esp_log.h"

static const char *TAG = "UART_LOG";
static int uart_baud_rate = 115200;
static int tx_pin = -1;
static int rx_pin = -1;
static bool is_ready = false;

/**
 * @brief 初始化 UART1（波特率、TX/RX引脚）
 */
void uart_log_init(int baud_rate, int tx_pin, int rx_pin) {
    if (baud_rate < 0 || tx_pin == -1 || rx_pin == -1) {
        ESP_LOGE(TAG, "无效参数");
        return;
    }
    
    uart_baud_rate = baud_rate;
    this->tx_pin = tx_pin;
    this->rx_pin = rx_pin;
}

/**
 * @brief 发送格式化日志
 */
void uart_log(const char *msg) {
    if (!is_ready) {
        ESP_LOGE(TAG, "UART 未初始化");
        return;
    }
    
    size_t len = strlen(msg);
    for (size_t i = 0; i < len; i++) {
        uart_driver_write(this->tx_pin, &msg[i], 1);
    }
}

/**
 * @brief 发送十六进制数据
 */
void uart_log_hex(uint32_t addr, uint8_t *data, size_t len) {
    if (!is_ready) {
        ESP_LOGE(TAG, "UART 未初始化");
        return;
    }
    
    char buf[10];
    sprintf(buf, "0x%08X: ", addr);
    uart_log(buf);
    
    for (size_t i = 0; i < len && i < 64; i++) {
        if (i > 0) {
            uart_log(" ");
        }
        char hex[3];
        sprintf(hex, "%02X", data[i]);
        uart_log(hex);
    }
}

/**
 * @brief 检查 UART 是否就绪
 */
bool uart_log_is_ready(void) {
    return is_ready;
}