#include "dht11.h"

DHT11* dhtInstance = nullptr;

void setup() {
    // Initialize DHT11 on pin 2 (example)
    if (!dhtInstance) {
        dhtInstance = new DHT11(2);
    }
}

void loop() {
    DHT11Reading reading;
    bool ok = dhtInstance->read(&reading);
    if (ok) {
        // Simple output – in real use you may want to publish via Serial or other means.
        Serial.print("Temp: ");
        Serial.println(reading.temperature, 1); // temperature in °C
        Serial.print("Humidity: ");
        Serial.println(reading.humidity, 1);      // humidity as %
    } else {
        Serial.println("Failed to read DHT11");
    }
}