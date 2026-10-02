#include <Arduino.h>

// Onboard LED is on pin 13 (PC13), active low
const int ledPin = 13;

void setup() {
    pinMode(ledPin, OUTPUT);
}

void loop() {
    digitalWrite(ledPin, LOW);   // turn ON
    delay(500);
    digitalWrite(ledPin, HIGH);  // turn OFF
    delay(500);
}
