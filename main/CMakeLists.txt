idf_component_register(SRCS "main.c"
                       REQUIRES bt nvs_flash esp_timer log esp_pm esp_hal_gpio
                       INCLUDE_DIRS ".")
