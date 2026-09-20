#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"

void app_main(void)
{
    printf("Hello world! from EspAiStudio cloud build\n");
    fflush(stdout);
    while (1) {
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}
