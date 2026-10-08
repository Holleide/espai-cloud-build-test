/*  ESP32-S3 HC-SR04 超声波测距 Arduino 应用 */
#include <Arduino.h>

class HC_SR04 {
public:
    void begin(uint8_t triggerPin, uint8_t echoPin) {
        pinMode(triggerPin, OUTPUT);
        pinMode(echoPin, INPUT);
    }

    /** 计算超声波测距（厘米） */
    float distanceCm(void) {
        unsigned long startus = micros();
        digitalWrite(triggerPin, LOW);
        delayMicroseconds(2);
        digitalWrite(triggerPin, HIGH);
        delayMicroseconds(10);
        digitalWrite(triggerPin, LOW);

        unsigned long duration = pulseDuration(echoPin);
        // speed of sound in cm/us = 29.1 (approx)
        float distanceUs = duration / 2.0;
        return distanceUs * 29.1f;   // convert us to cm
    }

private:
    uint8_t triggerPin, echoPin;

    /** 长度（微秒） */
    unsigned long pulseDuration(uint8_t pin) {
        pinMode(pin, INPUT);
        unsigned long start = micros();
        while (digitalRead(pin) == LOW) {
            // wait for echo edge
            start = micros();
        }
        return micros() - start;
    }
};

// 示例使用（可在 main.cpp 导入调用）：
// HC_SR04 sensor;
// sensor.begin(3, 2);   // trigger=GPIO3, echo=GPIO2
// float dist = sensor.distanceCm();
// Serial.print("Distance: "); Serial.println(dist, 1);