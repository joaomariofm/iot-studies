#include <WiFi.h>
#include <WebServer.h>
#include <ArduinoJson.h>

// WiFi credentials
const char* WIFI_SSID = "your-ssid-here";
const char* WIFI_PASSWORD = "your-password-here";

// Common variables
#define LED_BUILTIN 2
bool wifiConnected = false;

WebServer server(80);

void setup() {
	Serial.begin(9600);
	pinMode(LED_BUILTIN, OUTPUT);
	setup_wifi();
	setup_server();
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
}

void setup_server() {
	server.on("/led", HTTP_POST, handle_led);
	server.on("/led", HTTP_GET, handle_led_status);
	server.begin();
	Serial.println("HTTP server started");
}

void handle_led() {
	if (server.hasArg("plain")) {
		String body = server.arg("plain");
		StaticJsonDocument<200> doc;
		DeserializationError error = deserializeJson(doc, body);
		
		if (error) {
			server.send(400, "text/plain", "Invalid JSON");
			return;
		}

		if (doc.containsKey("state")) {
			bool ledState = doc["state"].as<bool>();
			digitalWrite(LED_BUILTIN, ledState ? HIGH : LOW);
			
			// Create JSON response
			StaticJsonDocument<200> response;
			response["success"] = true;
			response["state"] = ledState;
			
			String responseStr;
			serializeJson(response, responseStr);
			server.send(200, "application/json", responseStr);
		} else {
			server.send(400, "text/plain", "Missing 'state' field");
		}
	} else {
		server.send(400, "text/plain", "No data received");
	}
}

void handle_led_status() {
	StaticJsonDocument<200> doc;
	doc["state"] = (digitalRead(LED_BUILTIN) == HIGH);
	
	String response;
	serializeJson(doc, response);
	server.send(200, "application/json", response);
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
		wifiConnected = true;
	}
	
	if (wifiConnected) {
		server.handleClient();
	}
	
	delay(10); // Small delay to prevent watchdog timeout
}
