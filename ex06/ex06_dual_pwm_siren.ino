#define LED_A 2
#define LED_B 4

int step = 2;
int delayMs = 6;

void setup() {
  pinMode(LED_A, OUTPUT);
  pinMode(LED_B, OUTPUT);
}

void loop() {
  for (int b = 0; b <= 255; b += step) {
    analogWrite(LED_A, b);
    analogWrite(LED_B, 255 - b);
    delay(delayMs);
  }
  for (int b = 255; b >= 0; b -= step) {
    analogWrite(LED_A, b);
    analogWrite(LED_B, 255 - b);
    delay(delayMs);
  }
}
