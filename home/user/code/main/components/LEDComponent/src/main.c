void setup() {
  pinMode(LED_PIN, OUTPUT);
}

void loop() {
    digitalWrite(LED_PIN, HIGH);
    delay(1000); // 每秒闪烁一次
    digitalWrite(LED_PIN, LOW);
}
