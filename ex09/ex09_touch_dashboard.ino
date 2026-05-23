#include <WiFi.h>
#include <WebServer.h>

const char* ssid = "YOUR_SSID";
const char* password = "YOUR_PASSWORD";

const int touchPin = 4;
WebServer server(80);

const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="zh-CN">
<head>
  <meta charset="UTF-8" />
  <meta name="viewport" content="width=device-width, initial-scale=1.0" />
  <title>ESP32 触摸仪表盘</title>
  <style>
    body { margin: 0; min-height: 100vh; display: flex; flex-direction: column; justify-content: center; align-items: center; font-family: Arial, sans-serif; background: #081924; color: #f8f9fb; }
    .panel { width: 90%; max-width: 520px; padding: 28px; border-radius: 18px; background: rgba(255, 255, 255, 0.06); box-shadow: 0 0 24px rgba(0, 0, 0, 0.35); text-align: center; }
    .title { font-size: 2rem; margin-bottom: 16px; }
    .value { font-size: 5rem; margin: 20px 0; letter-spacing: 0.08em; }
    .hint { font-size: 1rem; color: #b8c7d2; }
  </style>
</head>
<body>
  <div class="panel">
    <div class="title">实时触摸传感器数值</div>
    <div class="value" id="touchValue">--</div>
    <div class="hint">将手指靠近触摸引脚，数值会变小。</div>
  </div>
  <script>
    const valueEl = document.getElementById('touchValue');

    async function refreshValue() {
      try {
        const response = await fetch('/touch');
        const text = await response.text();
        valueEl.textContent = text;
      } catch (error) {
        valueEl.textContent = 'ERR';
      }
    }

    setInterval(refreshValue, 250);
    window.addEventListener('load', refreshValue);
  </script>
</body>
</html>
)rawliteral";

void handleRoot() {
  server.send_P(200, "text/html", index_html);
}

void handleTouchValue() {
  int value = touchRead(touchPin);
  server.send(200, "text/plain", String(value));
}

void setup() {
  Serial.begin(115200);
  delay(100);

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

  server.on("/", handleRoot);
  server.on("/touch", handleTouchValue);
  server.onNotFound([]() { server.send(404, "text/plain", "Not found"); });
  server.begin();
}

void loop() {
  server.handleClient();
}
