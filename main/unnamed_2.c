# 工程配置：ESP-IDF v6 + esp32c3
idf_component_register(SRCS "main/codetest.c"
                        INCLUDE_DIRS "."
                      PRIV_REQUIRES esp_driver_gpio)

# 设置编译选项（可选）
target_compile_options(app PRIVATE -Wall -Wextra)