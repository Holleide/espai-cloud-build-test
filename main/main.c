splitdemo/
├── CMakeLists.txt
└── main/
    ├── CMakeLists.txt
    └── main.c          ← app_main() 入口点（初始化所有模块）
├── led/                ← LED 组件
│   ├── led.h
│   ├── led.c
│   └── CMakeLists.txt
├── button/             ← 按钮组件
│   ├── button.h
│   ├── button.c
│   └── CMakeLists.txt
└── uart_log/           ← UART 日志组件
    ├── uart_log.h
    ├── uart_log.c
    └── CMakeLists.txt