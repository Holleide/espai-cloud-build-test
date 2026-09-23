esp_demo_wifi_freertos/
├── CMakeLists.txt              # Build configuration (passing!)
├── main.c                      # Entry point - init sequence
└── components/
    ├── wifi_client/            # WiFi network task with DNS lookup
    │   └── CMakeLists.txt      # Component manifest
    ├── display/                # OLED SSD1306 driver + freefont rendering
    │   └── CMakeLists.txt      
    └── sensor_sim/             # Simulated I2C sensors for demo
        └── CMakeLists.txt