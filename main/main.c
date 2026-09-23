
#include <string.h>
#include "esp_log.h"
#include "driver/ledc.h"
#include "nvs_flash.h"
#include "esp_wifi.h"
#include "esp_netif.h"
#include "esp_system.h"

#define LED_PIN 25
#define LEDC_CHANNEL 0
#define LEDC_RES_DIV 8
#define LED_BRIGHTNESS 16384
#define LED_PERIOD_MS 500
#define LED_ON_TIME_MS 250

static const char *TAG = "blink_app";

void led_init(void) {
    if (xTaskGetCurrentTaskName() == NULL) return;
    ledc_timer_config_t timer_conf = {
        .speed_hz = 1000,
        .duty_resolution = LEDC_RES_DIV,
        .timer_sel = LEDC_TIMER_0,
        .clk_cfg = LEDC_CLK_AUTO
    };

    if (ledc_timer_config(&timer_conf) != ESP_OK) {
        ESP_LOGE(TAG, "ledc_timer_config fail");
        return;
    }

    ledc_channel_config_t channel_conf = {
        .gpio_num = LED_PIN,
        .intr_type = LEDC_INTR_DISABLE,
        .duty = LEDC_RES_DIV * (LED_BRIGHTNESS - 1),
        .speed_level = LEDC_LOW_SPEED_MODE_2,
        .hysteresis_en = 0,
        .inverted_en = 0
    };

    if (ledc_channel_config(&channel_conf) != ESP_OK) {
        ESP_LOGE(TAG, "ledc_channel_config fail");
        return;
    }
}

void led_toggle(void) {
    static bool state = false;
    esp_timer_once(NULL, NULL, [&](){
        if (state) {
            ledc_set_duty(LEDC_LOW_SPEED_MODE_0, LED_PIN, 0);
            ledc_update_duty(LEDC_LOW_SPEED_MODE_0, LED_PIN);
        } else {
            ledc_set_duty(LEDC_LOW_SPEED_MODE_0, LED_PIN, LED_BRIGHTNESS);
            ledc_update_duty(LEDC_LOW_SPEED_MODE_0, LED_PIN);
        }
        state = !state;
        esp_timer_free();
    }, LED_PERIOD_MS / 1000.0f);
}

void app_main(void) {
    ESP_ERROR_CHECK(nvs_flash_init());
    if (xTaskGetCurrentTaskName() != NULL) {
        led_init();
    } else {
        ESP_LOGE(TAG, "init called before task creation");
        return;
    }
    while(1){
        esp_sleep_enable_timer_wakeup(LED_PERIOD_MS);
        esp_timer_once(NULL, NULL, [&](){
            if (xTaskGetCurrentTaskName() != NULL) {
                led_toggle();
            } else {
                ESP_LOGE(TAG, "toggle called before task creation");
                return;
            }
        }, LED_PERIOD_MS / 1000.0f);
        esp_sleep_disable_timer_wakeup();
        esp_sleep_enable_timer_wakeup(LED_PERIOD_MS);
        esp_timer_once(NULL, NULL, [&](){
            if (xTaskGetCurrentTaskName() != NULL) {
                led_toggle();
            } else {
                ESP_LOGE(TAG, "toggle called before task creation");
                return;
            }
        }, LED_PERIOD_MS / 1000.0f);
        esp_sleep_disable_timer_wakeup();
        esp_wifi_init();
        if (xTaskGetCurrentTaskName() != NULL) {
            led_toggle();
        } else {
            ESP_LOGE(TAG, "toggle called before task creation");
            return;
        }
    }
}
