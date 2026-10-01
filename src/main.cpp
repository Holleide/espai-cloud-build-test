/*
 * ============================================================
 *  ESP32-C3 BLE HID 通用固件（熊大/FishR/蓝影 兼容协议）
 *  平台兼容：EasyClick EC / 斩月Lua / Bot.js Pro / AScript / 冰狐
 *  指令格式（明文文本，末尾必须带换行 \n）：
 *    click,x,y          点击
 *    swipe,x1,y1,x2,y2,ms   滑动
 *    press,x,y,ms       长按
 *    down,x,y           按下
 *    move,x,y           移动
 *    up                 抬起
 *  蓝牙设备名：ESP32-HID
 * ============================================================
 *  由 .ino 转为 .cpp：补 #include <Arduino.h> 与 parseCmd 前向声明
 *  （PlatformIO 的 .cpp 不做 Arduino IDE 式自动原型生成）。
 */

#include <Arduino.h>
#include <BleMouse.h>

void parseCmd(String cmd);   // 前向声明：loop() 里先于定义调用

// ========== 可配置参数 ==========
#define DEVICE_NAME      "ESP32-HID"   // 蓝牙设备名
#define DEVICE_MANUFACT  "Generic"     // 厂商名
#define RECONNECT_DELAY  3000          // 断线重连间隔(ms)
#define HEARTBEAT_LED    8             // 板载LED引脚(ESP32-C3多数是GPIO8)
// =================================

BleMouse bleMouse(DEVICE_NAME, DEVICE_MANUFACT, 100);

String rxBuf = "";
bool bleConnected = false;
unsigned long lastReconnectAttempt = 0;
unsigned long heartbeatTimer = 0;

// 串口接收缓冲
String serialBuf = "";

void setup() {
  Serial.begin(115200);
  delay(100);
  Serial.println();
  Serial.println("========================================");
  Serial.println(" ESP32-C3 BLE HID Firmware Starting...");
  Serial.println(" Device: " + String(DEVICE_NAME));
  Serial.println("========================================");

  pinMode(HEARTBEAT_LED, OUTPUT);
  digitalWrite(HEARTBEAT_LED, HIGH);

  bleMouse.begin();
  Serial.println("BLE started, waiting for connection...");
}

void loop() {
  // ===== BLE 连接状态检测与自动重连 =====
  bool currentState = bleMouse.isConnected();

  if (currentState && !bleConnected) {
    // 刚连接成功
    bleConnected = true;
    digitalWrite(HEARTBEAT_LED, LOW);
    Serial.println(">>> BLE Connected!");
  } else if (!currentState && bleConnected) {
    // 刚断开
    bleConnected = false;
    digitalWrite(HEARTBEAT_LED, HIGH);
    Serial.println(">>> BLE Disconnected, will retry...");
  }

  // 未连接时周期性尝试重连
  if (!bleConnected) {
    if (millis() - lastReconnectAttempt > RECONNECT_DELAY) {
      lastReconnectAttempt = millis();
      Serial.println("Attempting reconnect...");
      bleMouse.begin();  // 重新启动广播
      // LED闪烁提示未连接
      digitalWrite(HEARTBEAT_LED, (millis() / 200) % 2);
    }
  }

  // 已连接时LED常亮
  if (bleConnected) {
    digitalWrite(HEARTBEAT_LED, LOW);
  }

  // ===== BLE 接收数据（原厂 T-vK BleMouse v0.3.1 不支持）=====
  // 说明：T-vK/ESP32-BLE-Mouse 的 BleMouse 类是【纯 HID 输出设备】，
  // 头文件里只有 begin/end/isConnected/move/click/press/release/scroll 等，
  // 并没有 available()/read() 这类“接收主机回传文本”的流式接口——
  // 原 .ino 里的 bleMouse.available()/read() 依赖的是某个改过库的分支（未证实来源），
  // 用官方库编译会报 “'class BleMouse' has no member named 'available'”。
  // 因此本固件的指令注入统一走下面的【USB 串口通道】（原 sketch 已实现的备用通道），
  // 设备对手机/PC 仍表现为标准 BLE HID 鼠标，只是指令由串口下发。
  // BLE 侧仅用于输出 HID 报告，不接收命令。

  // ===== 串口接收数据（指令注入通道）=====
  while (Serial.available()) {
    char c = Serial.read();
    if (c == '\n' || c == '\r') {
      if (serialBuf.length() > 0) {
        parseCmd(serialBuf);
        serialBuf = "";
      }
    } else {
      serialBuf += c;
    }
  }

  delay(5);
}

/*
 * 指令解析器
 * 支持：click,x,y | swipe,x1,y1,x2,y2,ms | press,x,y,ms | down,x,y | move,x,y | up
 */
void parseCmd(String cmd) {
  cmd.trim();
  if (cmd.length() == 0) return;

  Serial.println("RX: " + cmd);

  // ---- click,x,y ----
  if (cmd.startsWith("click,")) {
    cmd.remove(0, 6);
    int comma = cmd.indexOf(',');
    if (comma < 0) return;
    int x = cmd.substring(0, comma).toInt();
    int y = cmd.substring(comma + 1).toInt();
    bleMouse.move(x, y);
    delay(20);
    bleMouse.press(MOUSE_LEFT);
    delay(30);
    bleMouse.release(MOUSE_LEFT);
    Serial.println("  -> click(" + String(x) + "," + String(y) + ")");
  }

  // ---- swipe,x1,y1,x2,y2,ms ----
  else if (cmd.startsWith("swipe,")) {
    cmd.remove(0, 6);
    int p1 = cmd.indexOf(',');
    int p2 = cmd.indexOf(',', p1 + 1);
    int p3 = cmd.indexOf(',', p2 + 1);
    int p4 = cmd.indexOf(',', p3 + 1);
    if (p1 < 0 || p2 < 0 || p3 < 0 || p4 < 0) return;

    int x1 = cmd.substring(0, p1).toInt();
    int y1 = cmd.substring(p1 + 1, p2).toInt();
    int x2 = cmd.substring(p2 + 1, p3).toInt();
    int y2 = cmd.substring(p3 + 1, p4).toInt();
    int t  = cmd.substring(p4 + 1).toInt();
    if (t < 10) t = 100;  // 最短滑动时间

    bleMouse.move(x1, y1);
    delay(20);
    bleMouse.press(MOUSE_LEFT);
    delay(30);
    // 分段滑动，模拟真实人手
    int steps = 10;
    int dx = (x2 - x1) / steps;
    int dy = (y2 - y1) / steps;
    int stepDelay = t / steps;
    if (stepDelay < 5) stepDelay = 5;
    for (int i = 1; i <= steps; i++) {
      bleMouse.move(x1 + dx * i, y1 + dy * i);
      delay(stepDelay);
    }
    delay(20);
    bleMouse.release(MOUSE_LEFT);
    Serial.println("  -> swipe(" + String(x1) + "," + String(y1) + " -> " +
                   String(x2) + "," + String(y2) + "," + String(t) + "ms)");
  }

  // ---- press,x,y,ms (长按) ----
  else if (cmd.startsWith("press,")) {
    cmd.remove(0, 6);
    int p1 = cmd.indexOf(',');
    int p2 = cmd.indexOf(',', p1 + 1);
    if (p1 < 0 || p2 < 0) return;
    int x = cmd.substring(0, p1).toInt();
    int y = cmd.substring(p1 + 1, p2).toInt();
    int holdMs = cmd.substring(p2 + 1).toInt();
    if (holdMs < 50) holdMs = 500;

    bleMouse.move(x, y);
    delay(20);
    bleMouse.press(MOUSE_LEFT);
    delay(holdMs);
    bleMouse.release(MOUSE_LEFT);
    Serial.println("  -> press(" + String(x) + "," + String(y) + "," + String(holdMs) + "ms)");
  }

  // ---- down,x,y (按下不松) ----
  else if (cmd.startsWith("down,")) {
    cmd.remove(0, 5);
    int comma = cmd.indexOf(',');
    if (comma < 0) return;
    int x = cmd.substring(0, comma).toInt();
    int y = cmd.substring(comma + 1).toInt();
    bleMouse.move(x, y);
    delay(20);
    bleMouse.press(MOUSE_LEFT);
    Serial.println("  -> down(" + String(x) + "," + String(y) + ")");
  }

  // ---- move,x,y (移动) ----
  else if (cmd.startsWith("move,")) {
    cmd.remove(0, 5);
    int comma = cmd.indexOf(',');
    if (comma < 0) return;
    int x = cmd.substring(0, comma).toInt();
    int y = cmd.substring(comma + 1).toInt();
    bleMouse.move(x, y);
    Serial.println("  -> move(" + String(x) + "," + String(y) + ")");
  }

  // ---- up (抬起) ----
  else if (cmd == "up") {
    bleMouse.release(MOUSE_LEFT);
    Serial.println("  -> up()");
  }

  // ---- help ----
  else if (cmd == "help") {
    Serial.println("Commands: click,x,y | swipe,x1,y1,x2,y2,ms | press,x,y,ms | down,x,y | move,x,y | up");
  }
}
