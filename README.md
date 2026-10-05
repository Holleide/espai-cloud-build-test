# hh

由 ESP-AI Studio 生成的 ESP-IDF 工程。

## 目录结构
- `CMakeLists.txt` — 顶层工程定义（include project.cmake + project(hh)）
- `main/` — 应用主组件
  - `main/CMakeLists.txt` — idf_component_register 源文件登记
  - `main/*.c` — 源码（app_main 入口）
- `sdkconfig.defaults` — 默认构建配置（目标芯片等）
- `.espai/config.json` — ESP-AI Studio 工程配置（芯片 / Flash / 云编译）

## 编译
本工程通过 ESP-AI Studio 的「云端编译（GitHub Actions）」构建，产物自动交付烧录工具。
本地开发可用 ESP-IDF：

```bash
idf.py set-target esp32s3
idf.py build
```