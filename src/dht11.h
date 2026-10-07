#ifndef DHT11_H
#define DHT11_H

#include <Arduino.h>

// DHT11 sensor type (enum for clarity)
enum SensorType { SENSOR_DHT11 };

// Define the DHT11 class structure
class DHT11 {
public:
    // Constructor takes a GPIO pin number
    explicit DHT11(int pin);
    
    // Read temperature and humidity
    // Returns true if read was successful, false otherwise.
    bool read(DHT11Reading* reading);
    
    // Get the current sensor type (for compatibility)
    SensorType getType() const;
    
private:
    int pin_;
    uint32_t lastReadTime_;
};

// Forward declaration of DHT11Reading for clarity
struct DHT11Reading {
    float temperature;   // in Celsius
    float humidity;      // as percentage
};

#endif // DHT11_H