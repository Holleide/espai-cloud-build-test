# ESP32-S3 BLE信标基站 - 构建说明

## 环境配置

本项目使用以下已安装的 ESP-IDF 工具链：

- **ESP-IDF**: `D:\.espressif\v6.0`
- **工具链**: `C:\Espressif\tools\`
  - CMake: `3.30.2`
  - Ninja: `1.12.1`
  - Xtensa 工具链：`esp-14.2.0_20251107`
  - Python 虚拟环境：`C:\Users\tao\.espressif\python_env\idf6.0_py3.13_env`

## 快速构建

### 方法 1：使用构建脚本（推荐）

```powershell
cd d:\.espressif\Beacon
.\build.ps1
```

### 方法 2：手动设置环境

```powershell
# 设置环境变量
$env:IDF_PATH = "D:\.espressif\v6.0"
$env:PATH = "C:\Espressif\tools\cmake\3.30.2\bin;" + `
            "C:\Espressif\tools\ninja\1.12.1;" + `
            "C:\Espressif\tools\xtensa-esp-elf\esp-14.2.0_20251107\xtensa-esp-elf\bin;" + `
            "C:\Espressif\tools\idf-exe\1.2;" + `
            "C:\Espressif\tools\ccache\4.10;" + `
            $env:PATH

# 切换到项目目录
cd d:\.espressif\Beacon

# 运行构建
C:\Users\tao\.espressif\python_env\idf6.0_py3.13_env\Scripts\python.exe $env:IDF_PATH\tools\idf.py build
```

## 其他常用命令

### 清理构建

```powershell
.\build.ps1  # 脚本会自动清理旧构建
# 或手动清理
Remove-Item -Recurse -Force build
```

### 烧录固件

```powershell
# 使用 idf.py 烧录
C:\Users\tao\.espressif\python_env\idf5.5_py3.13_env\Scripts\python.exe $env:IDF_PATH\tools\idf.py flash

# 或使用 esptool
```

### 查看串口输出

```powershell
C:\Users\tao\.espressif\python_env\idf5.5_py3.13_env\Scripts\python.exe $env:IDF_PATH\tools\idf.py monitor
```

### 配置项目

```powershell
C:\Users\tao\.espressif\python_env\idf5.5_py3.13_env\Scripts\python.exe $env:IDF_PATH\tools\idf.py menuconfig
```

## 常见问题解决

### 问题 1: "No module named 'esp_idf_monitor'"

**解决方案**：确保使用正确的 Python 虚拟环境
```powershell
C:\Users\tao\.espressif\python_env\idf5.5_py3.13_env\Scripts\python.exe
```

### 问题 2: CMake 配置失败，缺少子模块

**解决方案**：初始化 ESP-IDF 子模块
```powershell
cd D:\.espressif\v6.0
git submodule update --init --recursive
```

### 问题 3: 工具链未找到

**解决方案**：检查环境变量是否正确设置
```powershell
# 验证工具是否在 PATH 中
cmake --version
ninja --version
xtensa-esp32s3-elf-gcc --version
```

## 项目配置说明

主要配置文件：
- [`sdkconfig.defaults`](sdkconfig.defaults) - 默认配置
- [`main/CMakeLists.txt`](main/CMakeLists.txt) - 组件依赖
- [`partitions.csv`](partitions.csv) - 分区表

关键配置项：
- 目标芯片：ESP32-S3 (`CONFIG_IDF_TARGET="esp32s3"`)
- CPU 频率：160MHz
- BLE 使能
- 电源管理：启用
- 日志级别：INFO (3)

## 下一步

1. 等待 ESP-IDF 子模块初始化完成（首次需要较长时间）
2. 运行 `.\build.ps1` 进行构建
3. 连接 ESP32-S3 开发板
4. 烧录固件并测试

## 技术支持

如遇问题，请检查：
1. 所有工具是否正确安装
2. 环境变量是否正确配置
3. Python 依赖是否完整
4. ESP-IDF 子模块是否初始化

---
最后更新：2026-03-15
