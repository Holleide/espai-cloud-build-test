# GPIO blink 节省写法

用 esp_driver_gpio 的 gpio_set_level 配 vTaskDelay 实现闪烁，注意 LED_PIN 用 GPIO_NUM_2。
