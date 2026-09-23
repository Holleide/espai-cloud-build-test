/**
 * @file uart_log.h
 * @brief UART 日志模块接口定义
 * 
 * 功能：通过 UART1 发送格式化数据到电脑串口接收端
 */

#ifndef UART_LOG_H
#define UART_LOG_H

#ifdef __cplusplus
extern "C" {
#endif

void uart_log_init(int baud_rate, int tx_pin, int rx_pin);
void uart_log(const char *msg);
void uart_log_hex(uint32_t addr, uint8_t *data, size_t len);
bool uart_log_is_ready(void);

#ifdef __cplusplus
}
#endif

#endif // UART_LOG_H