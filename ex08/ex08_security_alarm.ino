#include <WiFi.h>
#include <WebServer.h>

const char* ssid = "YOUR_SSID";
const char* password = "YOUR_PASSWORD";

const int ledPin = 2;
const int touchPin = 4;
const unsigned long debounceMs = 80;

WebServer server(80);

bool armed = false;
bool alarmTriggered = false;

bool lastRawTouch = false;
bool stableTouch = false;
unsigned long lastDebounceTime = 0;
unsigned long lastBlinkTime = 0;
const unsigned long blinkInterval = 100;
bool blinkState = false;

uint16_t touchThreshold = 0;

bool readTouchPressed() {
  return touchRead(touchPin) < touchThreshold;
}

void updateAlarmState() {
  unsigned long now = millis();
  bool rawTouch = readTouchPressed();

  if (rawTouch != lastRawTouch) {
    lastDebounceTime = now;
    lastRawTouch = rawTouch;
  }

  if ((now - lastDebounceTime) >= debounceMs && rawTouch != stableTouch) {
    stableTouch = rawTouch;
    if (armed && stableTouch && !alarmTriggered) {
      alarmTriggered = true;
    }
  }

  if (alarmTriggered) {
    if (now - lastBlinkTime >= blinkInterval) {
      lastBlinkTime = now;
      blinkState = !blinkState;
      digitalWrite(ledPin, blinkState ? HIGH : LOW);
    }
  } else {
    digitalWrite(ledPin, LOW);
  }
}

const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="zh-CN">
<head>
  <meta charset="UTF-8" />
  <meta name="viewport" content="width=device-width, initial-scale=1.0" />
  <title>ESP32 安防报警器</title>
  <style>
    body { font-family: Arial, sans-serif; background: #121212; color: #f7f7f7; text-align: center; padding: 20px; }
    button { width: 120px; height: 50px; margin: 10px; font-size: 1rem; }
    .status { margin-top: 24px; font-size: 1.5rem; }
  </style>
</head>
<body>
  <h1>ESP32 安防报警器模拟</h1>
  <div>
    <button onclick="sendCommand('arm')">布防</button>
    <button onclick="sendCommand('disarm')">撤防</button>
  </div>
  <div class="status">状态：<span id="state">初始化中...</span></div>
  <script>
    const stateLabel = document.getElementById('state');

    function updateState(text) {
      stateLabel.textContent = text;
    }

    function sendCommand(cmd) {
      fetch('/' + cmd)
        .then(r => r.text())
        .then(text => updateState(text))
        .catch(console.error);
    }

    window.addEventListener('load', () => {
      sendCommand('status');
    });
  </script>
</body>
</html>
)rawliteral";

void handleRoot() {
  server.send_P(200, "text/html", index_html);
}

void handleArm() {
  armed = true;
  alarmTriggered = false;
  server.send(200, "text/plain", "已布防，触摸将触发报警");
}

void handleDisarm() {
  armed = false;
  alarmTriggered = false;
  digitalWrite(ledPin, LOW);
  server.send(200, "text/plain", "已撤防，报警已重置");
}

void handleStatus() {
  if (alarmTriggered) {
    server.send(200, "text/plain", "报警触发中，等待撤防");
  } else if (armed) {
    server.send(200, "text/plain", "已布防，等待触发");
  } else {
    server.send(200, "text/plain", "未布防");
  }
}

void setup() {
  Serial.begin(115200);
  delay(100);

  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  Serial.printf("Connecting to Wi-Fi %s", ssid);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print('.');
  }
  Serial.println();
  Serial.print("Connected, IP: ");
  Serial.println(WiFi.localIP());

  uint32_t total = 0;
  for (int i = 0; i < 32; i++) {
    total += touchRead(touchPin);
    delay(5);
  }
  uint16_t baseline = total / 32;
  touchThreshold = baseline * 3 / 4;
  if (touchThreshold < 10) touchThreshold = 10;
  Serial.printf("touch baseline=%u threshold=%u\n", baseline, touchThreshold);

  server.on("/", handleRoot);
  server.on("/arm", handleArm);
  server.on("/disarm", handleDisarm);
  server.on("/status", handleStatus);
  server.onNotFound([]() { server.send(404, "text/plain", "Not found"); });
  server.begin();
}

void loop() {
  server.handleClient();
  updateAlarmState();
}
