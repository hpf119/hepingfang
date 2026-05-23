const int ledPin = 2;
const int touchPin = 4;

const unsigned long debounceMs = 80;

uint16_t touchBaseline = 0;
uint16_t touchThreshold = 0;

bool lastRawTouch = false;
bool stableTouch = false;
bool ledState = false;

unsigned long lastDebounceTime = 0;

bool readTouchPressed() {
	return touchRead(touchPin) < touchThreshold;
}

void setup() {
	Serial.begin(115200);
	pinMode(ledPin, OUTPUT);
	digitalWrite(ledPin, LOW);

	delay(200);

	uint32_t total = 0;
	for (int i = 0; i < 32; i++) {
		total += touchRead(touchPin);
		delay(5);
	}

	touchBaseline = total / 32;
	touchThreshold = touchBaseline * 3 / 4;
	if (touchThreshold < 10) {
		touchThreshold = 10;
	}

	lastRawTouch = readTouchPressed();
	stableTouch = lastRawTouch;

	Serial.printf("touch baseline=%u threshold=%u\n", touchBaseline, touchThreshold);
}

void loop() {
	unsigned long now = millis();
	bool rawTouch = readTouchPressed();

	if (rawTouch != lastRawTouch) {
		lastDebounceTime = now;
		lastRawTouch = rawTouch;
	}

	if ((now - lastDebounceTime) >= debounceMs && rawTouch != stableTouch) {
		stableTouch = rawTouch;

		if (stableTouch) {
			ledState = !ledState;
			digitalWrite(ledPin, ledState ? HIGH : LOW);
		}
	}
}