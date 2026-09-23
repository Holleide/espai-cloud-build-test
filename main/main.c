task_setup(ADC_TASK, 500);      // ADC sampling every 1s  
task_setup(OLED_TASK, 3000);    // OLED display refresh every 3s
task_setup(WIFI_UPLOAD, 60000); // WiFi upload every 60s (background)