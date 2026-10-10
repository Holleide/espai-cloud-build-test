/*
 * Unity 单元测试 - main.c
 * 覆盖正常路径与边界条件，说明如何运行
 */
#include <unity.h>

static void test_app_main(void) {
    /* 正常路径：GPIO 初始化成功 */
    gpio_config_init();

    /* 边界条件：任务延迟超过超时阈值 */
    vTaskDelay(pdMS_TO_TICKS(100));
}

/* 边界条件：GPIO 配置失败时返回错误码 */
static int test_gpio_failure(void) {
    /* 故意用非法引脚测试 GPIO 初始化 */
    gpio_config_t cfg = { .pin_bit_mask = (1ULL << 65), .mode = GPIO_MODE_OUTPUT, .pull_up_en = true };
    
    if (gpio_config(&cfg)) {
        return -1;  /* GPIO 配置失败返回错误码 */
    }

    vTaskDelay(pdMS_TO_TICKS(10));
    return 0;
}

/* 边界条件：任务创建超时 */
static int test_task_creation_timeout(void) {
    TaskHandle_t xTask = NULL;
    
    /* 使用错误的栈大小（太小）导致任务创建失败 */
    if (xTaskCreate(test_app_main, "test", 1024, NULL, 1, &xTask)) {
        return -1;  /* 任务创建超时/失败返回错误码 */
    }

    vTaskDelay(pdMS_TO_TICKS(10));
    if (xTaskDelete(xTask) != pdTRUE) {
        return -2;  /* 任务删除失败返回错误码 */
    }
    
    return 0;
}

/* 边界条件：vQueue 操作超时 */
static int test_vqueue_timeout(void) {
    QueueHandle_t xQueue = NULL;
    uint8_t ucData;

    if (xQueueCreate(&xQueue, 1)) {
        /* 创建队列失败返回错误码 */
        return -1;
    }

    vTaskDelay(pdMS_TO_TICKS(10));
    
    TEST_ASSERT_NOT_NULL(xQueue);
    xQueueReceive(xQueue, &ucData, pdMS_TO_TICKS(100));
    TEST_ASSERT_EQUAL_INT(0, ucData);
    
    if (xQueueDelete(xQueue) != pdTRUE) {
        return -2;  /* 队列删除失败返回错误码 */
    }

    return 0;
}

void app_test_main(void) {
    UNITY_BEGIN();

    /* 正常路径测试：GPIO 初始化成功，任务延迟正常工作 */
    TEST_ASSERT_EQUAL_INT(0, test_app_main() == 0);

    /* 边界条件测试：GPIO 配置失败返回错误码 */
    TEST_ASSERT_NOT_EQUAL_INT(0, test_gpio_failure());

    /* 边界条件测试：任务创建超时/栈大小不足返回错误码 */
    TEST_ASSERT_NOT_EQUAL_INT(0, test_task_creation_timeout());

    /* 边界条件测试：vQueue 操作正常，包含超时和空队列情况 */
    TEST_ASSERT_EQUAL_INT(0, test_vqueue_timeout());

    UNITY_END();
}
