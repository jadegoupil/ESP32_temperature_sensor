#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_SHT31.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "config.h"
#include <WiFi.h>
#include <WebServer.h>

// Make a config.h file with your Wi-Fi credentials, like this:
// #define wifi_ssid "your-ssid"
// #define wifi_password "your-password"
// These will be used to connect the ESP32 to your Wi-Fi network in the setup() function. The ESP32 will then host a local web server that displays the temperature and humidity readings from the SHT31 sensor.

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_ADDR 0x3C

Adafruit_SHT31 sht = Adafruit_SHT31();
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

WebServer server(80);


// What the phone receives when it accesses the ESP32
void handleRoot() {

  float t = sht.readTemperature();
  float h = sht.readHumidity();

  String html = "<!DOCTYPE html>";
  html += "<html>";
  html += "<head>";
  html += "<meta name='viewport' content='width=device-width, initial-scale=1'>";
  html += "<meta http-equiv='refresh' content='5'>";
  html += "<title>ESP32 Sensor</title>";
  html += "</head>";

  html += "<body>";
  html += "<h1>ESP32 Sensor</h1>";

  if (isnan(t) || isnan(h)) {
    html += "<p>Sensor read error</p>";
  } else {
    html += "<h2>Temperature</h2>";
    html += "<p>" + String(t, 1) + " &deg;C</p>";

    html += "<h2>Humidity</h2>";
    html += "<p>" + String(h, 1) + " %</p>";
  }

  html += "</body>";
  html += "</html>";

  server.send(200, "text/html", html);
}


void setup() {

  Serial.begin(9600);

  Wire.begin(21, 22);


  // =========================
  // Wi-Fi
  // =========================

  WiFi.begin(wifi_ssid, wifi_password);

  Serial.print("Connecting to WiFi");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println();
  Serial.println("WiFi connected!");

  Serial.print("ESP32 IP address: ");
  Serial.println(WiFi.localIP());


  // =========================
  // Web server
  // =========================

  server.on("/", handleRoot);

  server.begin();

  Serial.println("Web server started!");


  // =========================
  // OLED
  // =========================

  if (!display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR)) {
    Serial.println("OLED not found, check wiring/address");
    while (true) delay(1000);
  }


  // =========================
  // SHT31 (temperature/humidity sensor)
  // =========================

  if (!sht.begin(0x44)) {
    Serial.println("SHT3x not found, check wiring/address");
    while (true) delay(1000);
  }

  display.setTextColor(SSD1306_WHITE);
}


unsigned long lastUpdate = 0;

void loop() {

  // Runs constantly, so the phone gets answered immediately
  server.handleClient();

  // Update OLED/serial every 10 s without blocking
  if (millis() - lastUpdate >= 10000) {
    lastUpdate = millis();

    float t = sht.readTemperature();
    float h = sht.readHumidity();

    display.clearDisplay();
    display.setTextSize(1);
    display.setCursor(0, 0);
    display.println("Temp / Humidity");

    display.setTextSize(2);
    display.setCursor(0, 20);

    if (isnan(t) || isnan(h)) {
      display.println("Read error");
    } else {
      display.printf("%.1f C\n", t);
      display.printf("%.0f %%", h);
      Serial.printf("Temperature: %.2f C | Humidity: %.1f %%\n", t, h);
    }

    display.display();
  }
}