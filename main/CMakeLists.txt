idf_component_register(
    SRCS "main.c"
    INCLUDE_DIRS "."
    REQUIRES 
        bt
        nvs_flash
        esp_timer
        log
        esp_pm
        esp_hal_gpio
)
