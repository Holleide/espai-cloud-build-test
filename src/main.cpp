#include <Arduino.h>

// 通用 Arduino 骨架：各板卡板载 LED 引脚不同。若该板核心未定义 LED_BUILTIN，
// 请把下面的 2 改成你板子原理图/丝印标注的实际 LED 引脚号。
#ifndef LED_BUILTIN
#define LED_BUILTIN 2
#endif

// 说明：本骨架【故意不调用 Serial】，以保证任意厂商 Arduino 核心都能开箱 `pio run`
// 通过链接——AVR/ESP32/RP2040 的 Serial 是 UART，但 Adafruit nRF52 的 Serial 是
// USB-CDC(Adafruit_USBD_CDC)，裸 pio run 会报 undefined reference to Serial 而链接失败。
// 需要串口输出时，请按你板子的官方例程自行加 Serial.begin(...)。
void setup() {
    pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
    digitalWrite(LED_BUILTIN, HIGH);
    delay(500);
    digitalWrite(LED_BUILTIN, LOW);
    delay(500);
}