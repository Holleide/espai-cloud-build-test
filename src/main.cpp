#include <Arduino.h>

// 新版 arduino-esp32 core 不再默认定义 LED_BUILTIN，缺失时回退到 esp32dev 板载 LED(GPIO2)
#ifndef LED_BUILTIN
#define LED_BUILTIN 2
#endif

void setup() {
    pinMode(LED_BUILTIN, OUTPUT);
    Serial.begin(115200);
    Serial.println("Hello from ESP-AI Studio (PlatformIO)");
}

void loop() {
    digitalWrite(LED_BUILTIN, HIGH);
    delay(1000);
    digitalWrite(LED_BUILTIN, LOW);
    delay(1000);
}
