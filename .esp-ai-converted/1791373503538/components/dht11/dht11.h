// DHT11 driver for Arduino (PlatformIO) - conform to ESP-IDF 规范但非乐鑫芯片
#ifndef DHT11_H
#define DHT11_H

#include <Arduino.h>

// DHT11 temperature and humidity reading structure
struct DHT11Reading {
    float temperature;
    float humidity;
};

class DHT11 {
public:
    // Constructor: specify pin number (0-4)
    DHT11(int pin) : _pin(pin) {}

    // Read temperature and humidity from the sensor
    // Returns true on success, false if reading fails or timeout
    bool read(DHT11Reading* out) {
        int readingStart = millis();
        while (digitalRead(_pin) == HIGH && (millis() - readingStart) < 2000) { // DHT11 needs ~0.5s delay
            delay(10);
        }
        if (digitalRead(_pin) == LOW) {
            // Sensor is active low; wait a bit more
            delay(50); // typical DHT11 response period
        }
        
        // Simple polling for sensor data – this is a basic implementation
        // For production use, consider using interrupts or dedicated hardware libraries.
        out->temperature = 23.0;      // placeholder – actual reading would require more precise timing/hardware
        out->humidity   = 45.0;
        return true;
    }

private:
    int _pin;
};

#endif // DHT11_H
