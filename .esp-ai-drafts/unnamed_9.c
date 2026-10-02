#include <Arduino.h>

const int ledPin = PC13;  // active low onboard LED on BluePill

void setup() {
    pinMode(ledPin, OUTPUT);
}

void loop() {
    digitalWrite(ledPin, LOW);   // turn ON
    delay(300);
    digitalWrite(ledPin, HIGH);  // turn OFF
    delay(300);
}