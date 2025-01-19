#include <WiFi.h>

// WiFi credentials
const char* WIFI_SSID = "your-ssid-here";
const char* WIFI_PASSWORD = "your-password-here";

// Common variables
#define LED_BUILTIN 2
bool wifiConnected = false;

void setup() {
	Serial.begin(9600);
	pinMode(LED_BUILTIN, OUTPUT);
	setup_wifi();
}

void setup_wifi() {
	delay(10);
	Serial.println();
	Serial.print("Connecting to ");
	Serial.println(WIFI_SSID);

	WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

	while (WiFi.status() != WL_CONNECTED) {
		delay(500);
		Serial.print(".");
	}

	Serial.println("");
	Serial.println("WiFi connected");
	Serial.println("IP address: ");
	Serial.println(WiFi.localIP());
	wifiConnected = true;
	digitalWrite(LED_BUILTIN, HIGH);
}

void loop() {
	// Check if WiFi is still connected
	if (WiFi.status() != WL_CONNECTED && wifiConnected) {
		Serial.println("WiFi connection lost!");
		digitalWrite(LED_BUILTIN, LOW);
		wifiConnected = false;
	} else if (WiFi.status() == WL_CONNECTED && !wifiConnected) {
		Serial.println("WiFi reconnected!");
		Serial.println("IP address: ");
		Serial.println(WiFi.localIP());
		digitalWrite(LED_BUILTIN, HIGH);
		wifiConnected = true;
	}
	delay(1000); // Check connection every second
}
