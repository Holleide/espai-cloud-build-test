/**
 * @file project_config.h
 * @brief 项目配置头文件
 * @details 包含所有项目特定的配置参数
 */

#ifndef PROJECT_CONFIG_H
#define PROJECT_CONFIG_H

#include "sdkconfig.h"

/* iBeacon参数配置 */
#ifndef CONFIG_IBEACON_MAJOR
#define CONFIG_IBEACON_MAJOR 1
#endif

#ifndef CONFIG_IBEACON_MINOR
#define CONFIG_IBEACON_MINOR 1
#endif

#ifndef CONFIG_IBEACON_TX_POWER
#define CONFIG_IBEACON_TX_POWER -59
#endif

#ifndef CONFIG_IBEACON_ADV_INTERVAL_MIN
#define CONFIG_IBEACON_ADV_INTERVAL_MIN 32
#endif

#ifndef CONFIG_IBEACON_ADV_INTERVAL_MAX
#define CONFIG_IBEACON_ADV_INTERVAL_MAX 64
#endif

/* LED指示配置 */
#ifndef CONFIG_BEACON_LED_GPIO
#define CONFIG_BEACON_LED_GPIO 2
#endif

#ifndef CONFIG_BEACON_LED_BLINK_MS
#define CONFIG_BEACON_LED_BLINK_MS 500
#endif

/* NVS存储配置 */
#ifndef CONFIG_NVS_NAMESPACE
#define CONFIG_NVS_NAMESPACE "beacon_cfg"
#endif

#ifndef CONFIG_NVS_KEY_MAJOR
#define CONFIG_NVS_KEY_MAJOR "major"
#endif

#ifndef CONFIG_NVS_KEY_MINOR
#define CONFIG_NVS_KEY_MINOR "minor"
#endif

#ifndef CONFIG_NVS_KEY_TX_POWER
#define CONFIG_NVS_KEY_TX_POWER "tx_power"
#endif

#endif /* PROJECT_CONFIG_H */
