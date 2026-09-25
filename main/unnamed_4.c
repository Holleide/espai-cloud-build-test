#include "codetest.h"

// 初始化标志位，避免重复配置
static bool s_led_initialized = false;

/**
 * @brief GPIO2 LED 初始化
 * 
 * @return ESP_OK 成功，ESP_ERR_INVALID_ARG 参数错误等
 */
esp_err_t codetest_init(void) {
    // 幂等性保护：如果已初始化则直接返回
    if (s_led_initialized) {
        return ESP_OK;
    }

    // 配置 GPIO2（pin 19）为输出模式
    gpio_config_t cfg = {
        .pin_bit_mask = (1ULL << LED_PIN),   // ⚠️ 必须用 1ULL，避免 32 位移位溢出
        .mode = GPIO_MODE_OUTPUT,             // 输出模式
        .pull_up_en = GPIO_PULLUP_DISABLE,    // 禁用上拉
        .pull_down_en = GPIO_PULLDOWN_DISABLE,// 禁用下拉
        .intr_type = GPIO_INTR_DISABLE        // 禁用中断（本例不需要）
    };

    ESP_ERROR_CHECK(gpio_config(&cfg));

    s_led_initialized = true;
    return ESP_OK;
}

/**
 * @brief LED 闪烁循环函数
 * 
 * 单次执行"动作 + vTaskDelay"，不写死循环。
 * 由 app_main 的 while(1) 驱动，便于日后挪进独立任务。
 */
void codetest_loop(void) {
    // 未初始化时直接返回并记录日志（不 panic）
    if (!s_led_initialized) {
        ESP_LOGE("codetest", "Not initialized, skipping");
        return;
    }

    // 关闭 LED（低电平点亮，因为 GPIO2 是内部上拉）
    gpio_set_level(LED_PIN, 0);
    
    // 延时 500ms
    vTaskDelay(pdMS_TO_TICKS(500));
    
    // 打开 LED
    gpio_set_level(LED_PIN, 1);

    // 再延时 500ms，完成一次闪烁周期（共 1s）
    vTaskDelay(pdMS_TO_TICKS(500));
}