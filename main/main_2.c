├── CMakeLists.txt          ← 顶层构建入口 (已完整)
├── main/
│   ├── CMakeLists.txt      ← 主组件注册 (main.cpp + "." include)
│   └── main.cpp            ← **应用入口**（含 app_main() / esp_main()）
├── src/                    ← 遗留目录，未被引用
├── include/                ← 头文件存放位置
└── sdkconfig.defaults      ← 芯片配置模板 (esp32c3)

【托管组件 managed_components/] — 0 个外部依赖