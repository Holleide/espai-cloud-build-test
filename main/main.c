/**
 * @file main.c
 * @brief ESP32-S3 BLE信标基站程序 (NimBLE高精度定位增强版)
 * @details 本文件实现BLE iBeacon信标基站功能，用于室内定位系统。
 *          信标持续广播iBeacon数据包，包含UUID、Major、Minor和TX Power信息。
 *          使用NimBLE协议栈，支持电源管理和电池监测。
 * @version 3.0.0
 * @date 2025-03-16
 * @author ESP32-S3定位系统开发团队
 */

#include <stdio.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_log.h"
#include "esp_timer.h"
#include "esp_pm.h"
#include "nvs_flash.h"
#include "nvs.h"
#include "soc/soc.h"
#include "soc/gpio_struct.h"
#include "hal/gpio_hal.h"

// ESP-IDF v6.0: NimBLE头文件路径已更改
#if CONFIG_BT_NIMBLE_ENABLED
#include "nimble/nimble_port.h"
#include "nimble/nimble_port_freertos.h"
#include "host/ble_hs.h"
#include "host/ble_gap.h"
#endif

#include "hal/gpio_hal.h"
#include "soc/soc_caps.h"

#define TAG "BEACON_MAIN"

/* ESP-IDF v6.0 GPIO兼容层 - 使用自定义类型名避免冲突 */
typedef enum {
    MY_GPIO_MODE_DISABLE = 0,
    MY_GPIO_MODE_INPUT = 1,
    MY_GPIO_MODE_OUTPUT = 2,
    MY_GPIO_MODE_INPUT_OUTPUT = 3,
    MY_GPIO_MODE_INPUT_OUTPUT_OD = 4,
    MY_GPIO_MODE_OUTPUT_OD = 5,
} my_gpio_mode_t;

typedef enum {
    MY_GPIO_PULLUP_ONLY,
    MY_GPIO_PULLDOWN_ONLY,
    MY_GPIO_PULLUP_PULLDOWN,
    MY_GPIO_FLOATING,
} my_gpio_pull_mode_t;

typedef enum {
    MY_GPIO_INTR_DISABLE = 0,
    MY_GPIO_INTR_POSEDGE = 1,
    MY_GPIO_INTR_NEGEDGE = 2,
    MY_GPIO_INTR_ANYEDGE = 3,
    MY_GPIO_INTR_LOW_LEVEL = 4,
    MY_GPIO_INTR_HIGH_LEVEL = 5,
} my_gpio_int_type_t;

typedef struct {
    uint64_t pin_bit_mask;
    my_gpio_mode_t mode;
    my_gpio_pull_mode_t pull_up_en;
    my_gpio_pull_mode_t pull_down_en;
    my_gpio_int_type_t intr_type;
} my_gpio_config_t;

#define MY_GPIO_PULLUP_ENABLE MY_GPIO_PULLUP_ONLY
#define MY_GPIO_PULLUP_DISABLE MY_GPIO_FLOATING
#define MY_GPIO_PULLDOWN_ENABLE MY_GPIO_PULLDOWN_ONLY
#define MY_GPIO_PULLDOWN_DISABLE MY_GPIO_FLOATING

typedef int my_gpio_num_t;

static inline esp_err_t my_gpio_config(const my_gpio_config_t *pGPIOConfig)
{
    gpio_hal_context_t hal;
    hal.dev = GPIO_LL_GET_HW(0);
    
    for (int i = 0; i < SOC_GPIO_PIN_COUNT; i++) {
        if (pGPIOConfig->pin_bit_mask & (1ULL << i)) {
            // 配置模式
            if (pGPIOConfig->mode == MY_GPIO_MODE_OUTPUT || pGPIOConfig->mode == MY_GPIO_MODE_OUTPUT_OD) {
                gpio_hal_output_enable(&hal, i);
            } else if (pGPIOConfig->mode == MY_GPIO_MODE_INPUT || pGPIOConfig->mode == MY_GPIO_MODE_INPUT_OUTPUT) {
                gpio_hal_input_enable(&hal, i);
            }
            
            // 配置开漏模式
            if (pGPIOConfig->mode == MY_GPIO_MODE_OUTPUT_OD || pGPIOConfig->mode == MY_GPIO_MODE_INPUT_OUTPUT_OD) {
                gpio_hal_od_enable(&hal, i);
            } else {
                gpio_hal_od_disable(&hal, i);
            }
            
            // 配置上拉/下拉
            if (pGPIOConfig->pull_up_en == MY_GPIO_PULLUP_ENABLE) {
                gpio_hal_pullup_en(&hal, i);
            } else {
                gpio_hal_pullup_dis(&hal, i);
            }
            
            if (pGPIOConfig->pull_down_en == MY_GPIO_PULLDOWN_ENABLE) {
                gpio_hal_pulldown_en(&hal, i);
            } else {
                gpio_hal_pulldown_dis(&hal, i);
            }
        }
    }
    
    return ESP_OK;
}

static inline void my_gpio_set_level(my_gpio_num_t gpio_num, uint32_t level)
{
    gpio_hal_context_t hal;
    hal.dev = GPIO_LL_GET_HW(0);
    gpio_hal_set_level(&hal, gpio_num, level);
}

/* 兼容性宏定义 - 映射到新的函数名 */
#define gpio_config my_gpio_config
#define gpio_set_level my_gpio_set_level
#define gpio_num_t my_gpio_num_t
#define gpio_config_t my_gpio_config_t
#define GPIO_MODE_OUTPUT MY_GPIO_MODE_OUTPUT
#define GPIO_PULLUP_DISABLE MY_GPIO_PULLUP_DISABLE
#define GPIO_PULLDOWN_DISABLE MY_GPIO_PULLDOWN_DISABLE
#define GPIO_INTR_DISABLE MY_GPIO_INTR_DISABLE

/* GPIO引脚号定义 */
#define GPIO_NUM_0   0
#define GPIO_NUM_1   1
#define GPIO_NUM_2   2
#define GPIO_NUM_3   3
#define GPIO_NUM_4   4
#define GPIO_NUM_5   5
#define GPIO_NUM_6   6
#define GPIO_NUM_7   7
#define GPIO_NUM_8   8
#define GPIO_NUM_9   9
#define GPIO_NUM_10  10

/* iBeacon UUID定义 (128位唯一标识符) */
#define IBEACON_UUID_BYTE_0   0x74
#define IBEACON_UUID_BYTE_1   0x27
#define IBEACON_UUID_BYTE_2   0x8B
#define IBEACON_UUID_BYTE_3   0xDA
#define IBEACON_UUID_BYTE_4   0xB6
#define IBEACON_UUID_BYTE_5   0x44
#define IBEACON_UUID_BYTE_6   0x45
#define IBEACON_UUID_BYTE_7   0x20
#define IBEACON_UUID_BYTE_8   0x8F
#define IBEACON_UUID_BYTE_9   0x0C
#define IBEACON_UUID_BYTE_10  0x72
#define IBEACON_UUID_BYTE_11  0x0E
#define IBEACON_UUID_BYTE_12  0xAF
#define IBEACON_UUID_BYTE_13  0x05
#define IBEACON_UUID_BYTE_14  0x99
#define IBEACON_UUID_BYTE_15  0x35

/* 默认信标参数配置 */
#define DEFAULT_MAJOR_VALUE       CONFIG_IBEACON_MAJOR
#define DEFAULT_MINOR_VALUE       CONFIG_IBEACON_MINOR
#define DEFAULT_TX_POWER          CONFIG_IBEACON_TX_POWER
#define DEFAULT_ADV_INTERVAL_MS   CONFIG_IBEACON_ADV_INTERVAL_MS
#define DEFAULT_POSITION_X        (CONFIG_IBEACON_POSITION_X / 100.0f)
#define DEFAULT_POSITION_Y        (CONFIG_IBEACON_POSITION_Y / 100.0f)

/* LED指示灯配置 */
#define BEACON_LED_GPIO          GPIO_NUM_2   /* LED连接的GPIO引脚 */
#define BEACON_LED_BLINK_MS      500          /* LED闪烁周期(毫秒) */

/* NVS存储命名空间和键名 */
#define NVS_NAMESPACE            "beacon_cfg"    /* NVS命名空间 */
#define NVS_KEY_MAJOR            "major"         /* Major值存储键 */
#define NVS_KEY_MINOR            "minor"         /* Minor值存储键 */
#define NVS_KEY_TX_POWER         "tx_power"      /* TX Power存储键 */
#define NVS_KEY_POSITION_X       "pos_x"         /* 位置X坐标存储键 */
#define NVS_KEY_POSITION_Y       "pos_y"         /* 位置Y坐标存储键 */

/**
 * @brief 信标配置结构体
 */
typedef struct {
    uint8_t uuid[16];       /* 128位UUID */
    uint16_t major;         /* Major值(2字节) */
    uint16_t minor;         /* Minor值(2字节) */
    int8_t tx_power;        /* 发射功率校准值 */
    float position_x;       /* 信标X坐标 (米) */
    float position_y;       /* 信标Y坐标 (米) */
} beacon_config_t;

/**
 * @brief 信标状态结构体
 */
typedef struct {
    bool advertising_active;    /* 广播状态 */
    uint32_t uptime_sec;        /* 运行时间(秒) */
    uint32_t adv_count;         /* 广播计数 */
} beacon_status_t;

/* 默认UUID常量 */
static const uint8_t s_default_uuid[16] = {
    IBEACON_UUID_BYTE_0, IBEACON_UUID_BYTE_1, IBEACON_UUID_BYTE_2, IBEACON_UUID_BYTE_3,
    IBEACON_UUID_BYTE_4, IBEACON_UUID_BYTE_5, IBEACON_UUID_BYTE_6, IBEACON_UUID_BYTE_7,
    IBEACON_UUID_BYTE_8, IBEACON_UUID_BYTE_9, IBEACON_UUID_BYTE_10, IBEACON_UUID_BYTE_11,
    IBEACON_UUID_BYTE_12, IBEACON_UUID_BYTE_13, IBEACON_UUID_BYTE_14, IBEACON_UUID_BYTE_15
};

/* 全局变量 */
static beacon_config_t s_beacon_config = {0};       /* 信标配置 */
static beacon_status_t s_beacon_status = {0};       /* 信标状态 */
static esp_timer_handle_t s_led_timer = NULL;       /* LED定时器句柄 */

/* 函数声明 */
static esp_err_t load_beacon_config(void);
static esp_err_t save_beacon_config(void);
static void led_timer_callback(void *arg);
static void print_beacon_info(void);
static void init_power_management(void);
static void ble_on_sync(void);
static void ble_on_reset(int reason);
static void ble_host_task(void *param);

/**
 * @brief 初始化电源管理 - 低功耗模式
 */
static void init_power_management(void)
{
    esp_pm_config_t pm_config = {
        .max_freq_mhz = 240,
        .min_freq_mhz = 40,
        .light_sleep_enable = false,
    };
    esp_pm_configure(&pm_config);
    ESP_LOGI(TAG, "Power mgmt: Dynamic frequency scaling (40-240MHz)");
}

/**
 * @brief NimBLE 同步回调 - 启动iBeacon广播 (暂时禁用)
 */
static void ble_on_sync(void)
{
    ESP_LOGW(TAG, "NimBLE功能暂时禁用 - 需要修复头文件路径");
    // TODO: 修复NimBLE头文件包含路径后启用此功能
#if 0
    ESP_LOGI(TAG, "NimBLE同步完成，启动iBeacon广播...");

    ble_hs_adv_fields fields = {0};
    fields.flags = BLE_HS_ADV_F_DISC_GEN | BLE_HS_ADV_F_BREDR_UNSUP;
    fields.tx_pwr_lvl = s_beacon_config.tx_power;
    fields.tx_pwr_lvl_is_present = 1;

    /* iBeacon厂商数据格式 */
    uint8_t ibeacon_data[25];
    ibeacon_data[0] = 0x4C;     /* Apple公司ID低字节 */
    ibeacon_data[1] = 0x00;     /* Apple公司ID高字节 */
    ibeacon_data[2] = 0x02;     /* iBeacon类型 */
    ibeacon_data[3] = 0x15;     /* iBeacon数据长度 */
    memcpy(&ibeacon_data[4], s_beacon_config.uuid, 16);
    ibeacon_data[20] = (s_beacon_config.major >> 8) & 0xFF;
    ibeacon_data[21] = s_beacon_config.major & 0xFF;
    ibeacon_data[22] = (s_beacon_config.minor >> 8) & 0xFF;
    ibeacon_data[23] = s_beacon_config.minor & 0xFF;
    ibeacon_data[24] = (uint8_t)s_beacon_config.tx_power;

    fields.mfg_data = ibeacon_data;
    fields.mfg_data_len = sizeof(ibeacon_data);

    int rc = ble_gap_adv_set_fields(&fields);
    if (rc != 0) {
        ESP_LOGE(TAG, "设置广播数据失败: %d", rc);
        return;
    }

    struct ble_gap_adv_params adv_params = {
        .conn_mode = BLE_GAP_CONN_MODE_NON,
        .disc_mode = BLE_GAP_DISC_MODE_GEN,
        .itvl_min = BLE_GAP_ADV_ITVL_MS(DEFAULT_ADV_INTERVAL_MS),
        .itvl_max = BLE_GAP_ADV_ITVL_MS(DEFAULT_ADV_INTERVAL_MS),
        .channel_map = 7,
    };

    rc = ble_gap_adv_start(BLE_OWN_ADDR_PUBLIC, NULL, BLE_HS_FOREVER, &adv_params, NULL, NULL);
    if (rc == 0) {
        s_beacon_status.advertising_active = true;
        ESP_LOGI(TAG, "iBeacon广播已启动");
    } else {
        ESP_LOGE(TAG, "广播启动失败: %d", rc);
    }
#endif
}

/**
 * @brief NimBLE 重置回调 (暂时禁用)
 */
static void ble_on_reset(int reason)
{
    ESP_LOGW(TAG, "BLE重置回调被调用 (reason=%d) - NimBLE功能暂时禁用", reason);
}

/**
 * @brief NimBLE 主任务 (暂时禁用)
 */
static void ble_host_task(void *param)
{
    ESP_LOGW(TAG, "NimBLE host task启动 - 但功能暂时禁用");
    vTaskDelete(NULL);  // 删除任务以避免阻塞
}

/**
 * @brief LED闪烁定时器回调函数
 * @param arg 未使用的参数
 */
static void led_timer_callback(void *arg)
{
    static bool s_led_state = false;
    s_led_state = !s_led_state;
    gpio_set_level(BEACON_LED_GPIO, s_led_state ? 1 : 0);
}

/**
 * @brief 从NVS加载信标配置
 * @details 从非易失性存储加载Major、Minor和TX Power配置
 * @return ESP_OK成功，其他值失败
 */
static esp_err_t load_beacon_config(void)
{
    nvs_handle_t nvs_handle;
    esp_err_t ret;

    /* 设置默认配置 */
    memcpy(s_beacon_config.uuid, s_default_uuid, 16);
    s_beacon_config.major = DEFAULT_MAJOR_VALUE;
    s_beacon_config.minor = DEFAULT_MINOR_VALUE;
    s_beacon_config.tx_power = DEFAULT_TX_POWER;
    s_beacon_config.position_x = DEFAULT_POSITION_X;
    s_beacon_config.position_y = DEFAULT_POSITION_Y;

    /* 打开NVS存储 */
    ret = nvs_open(NVS_NAMESPACE, NVS_READONLY, &nvs_handle);
    if (ret == ESP_ERR_NVS_NOT_FOUND) {
        ESP_LOGI(TAG, "No saved config found, using defaults");
        return save_beacon_config();
    }

    if (ret != ESP_OK) {
        ESP_LOGW(TAG, "Failed to open NVS: %s", esp_err_to_name(ret));
        return ret;
    }

    /* 读取Major值 */
    uint16_t major = 0;
    uint16_t minor = 0;
    int8_t tx_power = 0;

    ret = nvs_get_u16(nvs_handle, NVS_KEY_MAJOR, &major);
    if (ret == ESP_OK) {
        s_beacon_config.major = major;
    }

    /* 读取Minor值 */
    ret = nvs_get_u16(nvs_handle, NVS_KEY_MINOR, &minor);
    if (ret == ESP_OK) {
        s_beacon_config.minor = minor;
    }

    /* 读取TX Power值 */
    ret = nvs_get_i8(nvs_handle, NVS_KEY_TX_POWER, &tx_power);
    if (ret == ESP_OK) {
        s_beacon_config.tx_power = tx_power;
    }

    /* 读取位置坐标 */
    int32_t pos_x = 0, pos_y = 0;
    ret = nvs_get_i32(nvs_handle, NVS_KEY_POSITION_X, &pos_x);
    if (ret == ESP_OK) {
        s_beacon_config.position_x = pos_x / 100.0f;
    }
    ret = nvs_get_i32(nvs_handle, NVS_KEY_POSITION_Y, &pos_y);
    if (ret == ESP_OK) {
        s_beacon_config.position_y = pos_y / 100.0f;
    }

    nvs_close(nvs_handle);

    ESP_LOGI(TAG, "Loaded config from NVS: Major=%d, Minor=%d, TX=%d dBm, Pos=(%.2f, %.2f)",
             s_beacon_config.major, s_beacon_config.minor, s_beacon_config.tx_power,
             s_beacon_config.position_x, s_beacon_config.position_y);

    return ESP_OK;
}

/**
 * @brief 保存信标配置到NVS
 * @return ESP_OK成功，其他值失败
 */
static esp_err_t save_beacon_config(void)
{
    nvs_handle_t nvs_handle;
    esp_err_t ret;

    /* 打开NVS存储(读写模式) */
    ret = nvs_open(NVS_NAMESPACE, NVS_READWRITE, &nvs_handle);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "打开NVS写入失败: %s", esp_err_to_name(ret));
        return ret;
    }

    /* 保存配置项 */
    nvs_set_u16(nvs_handle, NVS_KEY_MAJOR, s_beacon_config.major);
    nvs_set_u16(nvs_handle, NVS_KEY_MINOR, s_beacon_config.minor);
    nvs_set_i8(nvs_handle, NVS_KEY_TX_POWER, s_beacon_config.tx_power);
    nvs_set_i32(nvs_handle, NVS_KEY_POSITION_X, (int32_t)(s_beacon_config.position_x * 100));
    nvs_set_i32(nvs_handle, NVS_KEY_POSITION_Y, (int32_t)(s_beacon_config.position_y * 100));

    /* 提交更改 */
    ret = nvs_commit(nvs_handle);
    nvs_close(nvs_handle);

    if (ret == ESP_OK) {
        ESP_LOGI(TAG, "Config saved to NVS");
    }

    return ret;
}

/**
 * @brief 打印信标信息
 */
static void print_beacon_info(void)
{
    ESP_LOGI(TAG, "========================================");
    ESP_LOGI(TAG, "ESP32-S3 BLE Beacon Station (NimBLE)");
    ESP_LOGI(TAG, "========================================");
    ESP_LOGI(TAG, "UUID: %02X%02X%02X%02X-%02X%02X-%02X%02X-%02X%02X-%02X%02X%02X%02X%02X%02X",
             s_beacon_config.uuid[0], s_beacon_config.uuid[1],
             s_beacon_config.uuid[2], s_beacon_config.uuid[3],
             s_beacon_config.uuid[4], s_beacon_config.uuid[5],
             s_beacon_config.uuid[6], s_beacon_config.uuid[7],
             s_beacon_config.uuid[8], s_beacon_config.uuid[9],
             s_beacon_config.uuid[10], s_beacon_config.uuid[11],
             s_beacon_config.uuid[12], s_beacon_config.uuid[13],
             s_beacon_config.uuid[14], s_beacon_config.uuid[15]);
    ESP_LOGI(TAG, "Major: %d", s_beacon_config.major);
    ESP_LOGI(TAG, "Minor: %d", s_beacon_config.minor);
    ESP_LOGI(TAG, "TX Power: %d dBm", s_beacon_config.tx_power);
    ESP_LOGI(TAG, "Position: (%.2f, %.2f) meters", s_beacon_config.position_x, s_beacon_config.position_y);
    ESP_LOGI(TAG, "Adv Interval: %d ms", DEFAULT_ADV_INTERVAL_MS);
    ESP_LOGI(TAG, "========================================");
}

/**
 * @brief 初始化LED指示灯
 * @return ESP_OK成功，其他值失败
 */
static esp_err_t led_init(void)
{
    /* 配置GPIO */
    gpio_config_t io_conf = {
        .pin_bit_mask = (1ULL << BEACON_LED_GPIO),
        .mode = GPIO_MODE_OUTPUT,
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE,
    };

    esp_err_t ret = gpio_config(&io_conf);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "LED GPIO配置失败: %s", esp_err_to_name(ret));
        return ret;
    }

    /* 创建LED闪烁定时器 */
    const esp_timer_create_args_t led_timer_args = {
        .callback = &led_timer_callback,
        .name = "led_blink"
    };

    ret = esp_timer_create(&led_timer_args, &s_led_timer);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "LED定时器创建失败: %s", esp_err_to_name(ret));
        return ret;
    }

    /* 启动定时器 */
    ret = esp_timer_start_periodic(s_led_timer, BEACON_LED_BLINK_MS * 1000);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "LED定时器启动失败: %s", esp_err_to_name(ret));
        return ret;
    }

    return ESP_OK;
}

/**
 * @brief 主函数入口
 */
void app_main(void)
{
    ESP_LOGI(TAG, "ESP32-S3 BLE Beacon Starting...");
    ESP_LOGI(TAG, "Version: 3.0.0");

    /* Initialize NVS */
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_LOGW(TAG, "Erasing NVS partition...");
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);
    printf("NVS OK\n");
    vTaskDelay(pdMS_TO_TICKS(100));

    /* Load beacon config */
    ESP_LOGI(TAG, "Loading beacon config...");
    ret = load_beacon_config();
    if (ret != ESP_OK) {
        ESP_LOGW(TAG, "Using default config");
    }
    ESP_LOGI(TAG, "Config loaded");

    /* Print beacon info */
    ESP_LOGI(TAG, "Printing beacon info...");
    print_beacon_info();
    ESP_LOGI(TAG, "Info printed");

    /* Init power management */
    ESP_LOGI(TAG, "Init power management...");
    init_power_management();
    ESP_LOGI(TAG, "Power mgmt initialized");

    /* NimBLE disabled */
    ESP_LOGW(TAG, "NimBLE disabled - BLE advertising not started");

    /* LED disabled */
    ESP_LOGW(TAG, "LED disabled");

    ESP_LOGI(TAG, "System started! Free heap: %lu bytes", (unsigned long)esp_get_free_heap_size());

    /* Main loop */
    while (1) {
        vTaskDelay(pdMS_TO_TICKS(10000));
        ESP_LOGI(TAG, "Running... Free heap: %lu", (unsigned long)esp_get_free_heap_size());
    }
}
