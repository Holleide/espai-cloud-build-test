#include "led_flash.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"

static const char *TAG = "LED_FLASH";

/** 内部任务函数 */
static void led_flash_task(void *pvParameters) {
    int period_ms = (int)(uintptr_t)pvParameters;
    
    ESP_LOGI(TAG, "LED flash task started");
    
    while (1) {
        // LED ON
        ESP_LOGI(TAG, "LED ON");
        gpio_set_level(led_pin, 1);
        
        vTaskDelay(pdMS_TO_TICKS(period_ms));
        
        // LED OFF
        ESP_LOGI(TAG, "LED OFF");
        gpio_set_level(led_pin, 0);
        
        vTaskDelay(pdMS_TO_TICKS(period_ms));
    }
}

esp_err_t led_flash_start(int led_pin, int flash_period_ms) {
    esp_err_t ret = ESP_OK;
    
    // 参数校验
    if (led_pin < 0 || led_pin > 45) {
        ESP_LOGE(TAG, "Invalid GPIO pin: %d", led_pin);
        return ESP_ERR_INVALID_ARG;
    }
    
    if (flash_period_ms <= 0) {
        ESP_LOGE(TAG, "Invalid flash period: %d ms", flash_period_ms);
        return ESP_ERR_INVALID_ARG;
    }
    
    // 初始化 GPIO
    gpio_init(led_pin);
    gpio_set_direction(led_pin, GPIO_MODE_IN);
    gpio_set_pull_down(led_pin, true);
    gpio_set_pull_up(led_pin, false);
    
    // 创建并启动任务
    xTaskCreatePinnedToCore(
        led_flash_task,
        "LED_Flash",
        flash_period_ms * 2 + 100,  // 栈大小：周期*2+100（安全余量）
        (void*)(uintptr_t)flash_period_ms,
        5,  // 优先级
        NULL,
        1   // 绑定到当前运行核心（ESP32-S3 双核适用）
    );
    
    if (xTaskCreatePinnedToCore == NULL) {
        ESP_LOGE(TAG, "Failed to create LED flash task");
        return ESP_ERR_INVALID_STATE;
    }
    
    ESP_LOGI(TAG, "LED flash started on GPIO%d, period=%dms", led_pin, flash_period_ms);
    return ret;
}

void led_flash_stop(int led_pin, int flash_period_ms) {
    // 关闭 LED
    gpio_set_level(led_pin, 0);
    
    ESP_LOGI(TAG, "LED flash stopped");
}