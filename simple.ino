#define LED_BUILTIN 2

void setup() {
	Serial.begin(9600);
	pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
	digitalWrite(LED_BUILTIN, HIGH);
	delay(1000);
	digitalWrite(LED_BUILTIN, LOW);
	delay(1000);
}
