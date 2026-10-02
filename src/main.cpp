#include <Arduino.h>

// Onboard LED is on pin PC13, active low (BluePill STM32F103C8T6).
// Use the core pin-name constant PC13 so it maps unambiguously to port C pin 13
// (raw digital "13" is not guaranteed to be the LED on the generic stm32duino variant).
const int ledPin = PC13;

void setup() {
    pinMode(ledPin, OUTPUT);
}

void loop() {
    digitalWrite(ledPin, LOW);   // turn ON
    delay(500);
    digitalWrite(ledPin, HIGH);  // turn OFF
    delay(500);
}
