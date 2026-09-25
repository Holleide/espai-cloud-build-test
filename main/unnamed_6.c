---

## 📋 功能清单总结

| 模块 | 功能 | 实现方式 |
|------|------|---------|
| **初始化** | GPIO2 配置为输出 | `codetest_init()` + `gpio_config()` + 幂等保护 |
| **循环** | LED 闪烁（关→开） | `codetest_loop()` + `vTaskDelay(1000ms)` |
| **错误处理** | 统一检查返回值 | `ESP_ERROR_CHECK()` / `if (ret != ESP_OK)` |
| **日志输出** | 记录状态 | `ESP_LOGI/E/W` 标签 `TAG` |

---

## ⚠️ 重要注意事项（v6 规范）

1. ✅ **GPIO2 = pin 19** - ESP32C3 的 GPIO2 对应物理引脚 19
2. ✅ **必须用 `1ULL << LED_PIN`** - 避免 32 位位移溢出导致所有位被设置
3. ✅ **组件名是 `esp_driver_gpio`** - 不是过时的 umbrella `driver`
4. ✅ **SRCS 补全 `main/` 目录** - 否则 CMakeLists 中的 SRCS 不会自动包含子文件
5. ✅ **函数返回 `esp_err_t`，由 app_main 统一检查** - 不 panic

---

## 🎯 验证方法

烧录后通过串口查看：