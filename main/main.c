// main.c
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

void sensor_task(void *arg);   // 传感器采集任务
void mqtt_task(void *arg);     // MQTT 上报任务

void app_main(void) {
    xTaskCreate(sensor_task, "sensor", 4096, NULL, 5, NULL);
    xTaskCreate(mqtt_task,   "mqtt",   4096, NULL, 5, NULL);
}