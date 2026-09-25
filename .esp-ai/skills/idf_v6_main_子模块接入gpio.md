## 适用场景
在本工程（codertest，ESP-IDF **v6.0.1**，目标芯片 **esp32c3**）里新增一个 `main/<模块>/<模块>.c + .h` 形式的功能模块并使用外设驱动。

## 关键步骤
1. **头文件不再自动传递 FreeRTOS**
   v6 起外设驱动头（如 `driver/gpio.h`）不再间接引入 FreeRTOS 头。只要用到 `vTaskDelay` / `pdMS_TO_TICKS` / 队列 / 信号量，就必须显式：
   ```c
   #include "freertos/FreeRTOS.h"
   #include "freertos/task.h"
   ```
   漏掉会报 `vTaskDelay undeclared`。

2. **umbrella 组件 `driver` 已弃用**
   `REQUIRES "driver"` 不再聚合各驱动。GPIO 要用组件名 **`esp_driver_gpio`**（头文件路径仍是 `driver/gpio.h`，二者不冲突）。

3. **CMakeLists 必须同时补三处**（只加 SRCS 会编译过、但 `#include "xxx.h"` 找不到）
   ```cmake
   idf_component_register(SRCS "main.c"
                               "codetest/codetest.c"
                          INCLUDE_DIRS "."
                                       "codetest"
                          PRIV_REQUIRES esp_driver_gpio)
   ```
   注：用 `batch_write_files` 写 `main/` 下文件时会自动登记 SRCS，但 **INCLUDE_DIRS / REQUIRES 仍需手写补全**。

4. **模块接口约定（本工程已采用）**
   - `esp_err_t xxx_init(void)`：返回错误码而**不是**内部 `ESP_ERROR_CHECK`，把是否 panic 的决定权交给调用方；在 `app_main` 里再 `ESP_ERROR_CHECK(xxx_init())`。
   - `void xxx_loop(void)`：**单次**执行"动作 + vTaskDelay"，不写死循环；由 `app_main` 的 `while(1)` 驱动，便于日后挪进独立任务。
   - 用 `static bool s_inited` 做幂等保护，`loop` 里未初始化直接 `ESP_LOGE` 返回。

5. **GPIO 写法**
   ```c
   gpio_config_t io = {
       .pin_bit_mask = (1ULL << GPIO_NUM),  // 必须 1ULL，否则 32 位移位溢出
       .mode = GPIO_MODE_OUTPUT,
       .pull_up_en = GPIO_PULLUP_DISABLE,
       .pull_down_en = GPIO_PULLDOWN_DISABLE,
       .intr_type = GPIO_INTR_DISABLE,
   };
   ```

## 注意事项 / 踩坑
- **esp32c3 的 GPIO2 是 strapping 引脚**：上电瞬间为低会进下载模式。拿它做 LED 输出时若出现"烧录后不复位/一直启动失败"，先改引脚（如 IO8）再怀疑代码。
- `vTaskDelay` 不可省：`app_main` 里忙等会触发 task watchdog，且 IDLE 任务无法运行。
- 翻转失败时回滚软件状态变量，保持 `s_led_state` 与实际电平一致。