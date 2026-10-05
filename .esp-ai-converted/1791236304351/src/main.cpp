#include <Arduino.h>

// 定义LED引脚。请根据您实际的STM32F103开发板的引脚定义进行修改。
// 假设我们将一个GPIO作为LED输出，例如使用板载LED或外部连接的引脚。
// 请查阅您的硬件原理图确定正确的GPIO编号。
const int ledPin = 2; // 示例：假设使用GPIO2作为LED控制（具体请根据STM32F103板卡定义修改）

void setup() {
    // 初始化串口通信，用于调试输出
    Serial.begin(115200);
    while (!Serial) {
        // 等待串口连接，如果调试器未连接则会阻塞
    }
    Serial.println("STM32F103 LED Blink Test Start!");

    // 配置LED引脚为输出模式
    pinMode(ledPin, OUTPUT);

    // 初始化时所有外设（如GPIO）配置完成
}

void loop() {
    // 点亮LED
    digitalWrite(ledPin, HIGH);
    Serial.println("LED ON");
    delay(1000); // 延时1秒

    // 熄灭LED
    digitalWrite(ledPin, LOW);
    Serial.println("LED OFF");
    delay(1000); // 延时1秒
}