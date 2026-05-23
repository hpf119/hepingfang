
#define TOUCH_PIN 4
#define LED_PIN 2

const int TOUCH_THRESHOLD = 40;
const unsigned long DEBOUNCE_MS = 150;

int speedLevel = 1;
bool lastTouched = false;
unsigned long lastBounceTime = 0;

void setup() {
  pinMode(LED_PIN, OUTPUT);
}

bool readTouch() {
#ifdef ARDUINO_ARCH_ESP32
  return touchRead(TOUCH_PIN) < TOUCH_THRESHOLD;
#else
  return digitalRead(TOUCH_PIN) == HIGH;
#endif
}

void loop() {
  bool curTouched = readTouch();
  unsigned long now = millis();
  if (curTouched != lastTouched) {
    if (now - lastBounceTime >= DEBOUNCE_MS) {
      if (curTouched && !lastTouched) {
        speedLevel++;
        if (speedLevel > 3) speedLevel = 1;
      }
      lastBounceTime = now;
      lastTouched = curTouched;
    }
  }

  int step;
  int delayMs;
  switch (speedLevel) {
    case 1: step = 1; delayMs = 8; break;
    case 2: step = 3; delayMs = 5; break;
    default: step = 6; delayMs = 2; break;
  }

  for (int b = 0; b <= 255; b += step) {
    analogWrite(LED_PIN, b);
    delay(delayMs);
    if (readTouch()) break;
  }
  for (int b = 255; b >= 0; b -= step) {
    analogWrite(LED_PIN, b);
    delay(delayMs);
    if (readTouch()) break;
  }
}
