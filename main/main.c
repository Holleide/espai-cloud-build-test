#define LED_PIN 2 // LED引脚编号
void setup() {
  pinMode(LED_PIN, OUTPUT);
}
void loop() {
  digitalWrite(LED_PIN, HIGH); // 点亮LED
  delay(1000); // 每秒闪烁一次
  digitalWrite(LED_PIN, LOW); // 关闭LED
}