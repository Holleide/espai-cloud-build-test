// File: src/main.c
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

static const char *TAG = "main";

void app_main(void) {
    printf("Hello from STM32F103 example\n");
    while (1) { vTaskDelay(pdMS_TO_TICKS(1000)); }
}