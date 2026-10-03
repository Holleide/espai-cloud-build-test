# ___

由 ESP-AI Studio 生成的 PlatformIO 工程（平台 ststm32 · 板卡 genericSTM32F103C8 · 框架 Arduino）。

## 目录结构
- `platformio.ini` — 工程与板卡定义（platform=ststm32, board=genericSTM32F103C8, framework=arduino）
- `src/` — 源码入口
- `lib/` — 工程私有库
- `include/` — 公共头文件
- `.espai/config.json` — ESP-AI Studio 工程配置（含 projectType=platformio）

## 编译

本工程通过 ESP-AI Studio 的「云端编译（GitHub Actions · PlatformIO）」构建，
等价命令为：

```bash
pio run
```