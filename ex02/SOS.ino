const int ledPin = 4;


const unsigned long DOT_MS = 100;     // ·
const unsigned long DASH_MS = 300;    // —
const unsigned long GAP_MS = 100;     // 闪烁间隔
const unsigned long PAUSE_MS = 1200;  // SOS末尾停顿

const int pattern[] = {0,0,0, 1,1,1, 0,0,0};
const int patternLen = 9;

unsigned long lastTime = 0;
int idx = 0;
bool ledIs = false;
unsigned long onTime = 0;

void setup() {
	pinMode(ledPin, OUTPUT);
	digitalWrite(ledPin, LOW);
}

void loop() {
	unsigned long now = millis();
	unsigned long elapsed = now - lastTime;

	if (ledIs) {
		// LED亮的阶段
		if (elapsed >= onTime) {
			ledIs = false;
			lastTime = now;
			digitalWrite(ledPin, LOW);
		}
	} else {
		// LED灭的阶段
		unsigned long wait = (idx >= patternLen) ? PAUSE_MS : GAP_MS;

		if (elapsed >= wait) {
			if (idx >= patternLen) {
				idx = 0;
			}

			ledIs = true;
			onTime = (pattern[idx] == 0) ? DOT_MS : DASH_MS;
			lastTime = now;
			digitalWrite(ledPin, HIGH);
			idx++;
		}
	}
}
