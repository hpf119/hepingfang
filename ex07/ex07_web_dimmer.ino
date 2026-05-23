#include <WiFi.h>
#include <WebServer.h>

const char* ssid = "YOUR_SSID";
const char* password = "YOUR_PASSWORD";

const int ledPin = 2;
const int pwmChannel = 0;
const int pwmFreq = 5000;
const int pwmResolution = 8;

WebServer server(80);

const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="zh-CN">
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0">
  <title>ESP32 无极调光器</title>
  <style>
    body { background: #101820; color: #f5f5f5; font-family: Arial, sans-serif; margin: 0; padding: 24px; display: flex; justify-content: center; align-items: center; min-height: 100vh; }
    .card { width: 100%; max-width: 520px; background: rgba(255,255,255,0.08); border-radius: 18px; padding: 28px; box-shadow: 0 20px 60px rgba(0,0,0,0.35); text-align: center; }
    h1 { margin: 0 0 18px; font-size: 2rem; }
    input[type=range] { width: 100%; margin-top: 24px; }
    .value { margin-top: 18px; font-size: 2rem; }
  </style>
</head>
<body>
  <div class="card">
    <h1>ESP32 无极调光器</h1>
    <input id="brightnessSlider" type="range" min="0" max="255" value="0">
    <div class="value">亮度: <span id="brightnessValue">0</span></div>
  </div>
  <script>
    const slider = document.getElementById('brightnessSlider');
    const valueLabel = document.getElementById('brightnessValue');

    function sendBrightness(value) {
      valueLabel.textContent = value;
      fetch('/set?value=' + value).catch(error => console.error(error));
    }

    slider.addEventListener('input', event => {
      sendBrightness(event.target.value);
    });

    window.addEventListener('load', () => {
      sendBrightness(slider.value);
    });
  </script>
</body>
</html>
)rawliteral";

void handleRoot() {
  server.send_P(200, "text/html", index_html);
}

void handleSetBrightness() {
  if (!server.hasArg("value")) {
    server.send(400, "text/plain", "Missing value");
    return;
  }

  int value = server.arg("value").toInt();
  value = constrain(value, 0, 255);
  ledcWrite(pwmChannel, value);
  server.send(200, "text/plain", String(value));
}

void setup() {
  Serial.begin(115200);
  delay(100);

  pinMode(ledPin, OUTPUT);
  ledcSetup(pwmChannel, pwmFreq, pwmResolution);
  ledcAttachPin(ledPin, pwmChannel);
  ledcWrite(pwmChannel, 0);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  Serial.printf("Connecting to Wi-Fi: %s\n", ssid);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print('.');
  }

  Serial.println();
  Serial.print("Connected. IP address: ");
  Serial.println(WiFi.localIP());

  server.on("/", handleRoot);
  server.on("/set", handleSetBrightness);
  server.onNotFound([]() { server.send(404, "text/plain", "Not found"); });
  server.begin();
}

void loop() {
  server.handleClient();
}
