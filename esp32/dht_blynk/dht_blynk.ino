#define BLYNK_TEMPLATE_ID "TMPL6-7KRXnIF"
#define BLYNK_TEMPLATE_NAME "HomeAutomation"
#define BLYNK_AUTH_TOKEN "P2unbo5T9u1WxfcNA8OoL8Okn1m-nwuS"

#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include <string.h>
#include <ESP32Servo.h>
#include "DHT.h"

// DHT define
#define DHTTYPE DHT11   // DHT 11
#define DHTPIN 5
DHT dht(DHTPIN, DHTTYPE);

// Define LED
#define LED 2
WidgetLED LED_ON_APP(V0);
int button;

const int servoPin = 13;
Servo servo;
int pos = 0;

char auth[] = BLYNK_AUTH_TOKEN;
// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "Pixel";
char pass[] = "12345678";

void setup()
{
  // Debug console
  pinMode(LED, OUTPUT);
  Serial.begin(115200);
  dht.begin();
  servo.attach(servoPin, 500, 2400);
  Blynk.begin(auth, ssid, pass);
}

BLYNK_WRITE(V1) {
  button = param.asInt();
  if(button == 1) {
    digitalWrite(LED, HIGH);
    LED_ON_APP.on();
  } else {
    digitalWrite(LED, LOW);
    LED_ON_APP.off();
  }
}

BLYNK_WRITE(V2) {
  button = param.asInt();
  if(button == 1) {
    // for (pos = 0; pos <= 180; pos += 1) {
    //   servo.write(pos);
    //   delay(15);
    servo.write(90);
  }
  else {
    // for (pos = 180; pos >= 0; pos -= 1) {
    // servo.write(pos);
    // delay(15);
    servo.write(0);
    }
}

void loop()
{
  Blynk.run();
  // Read Temp
  float t = dht.readTemperature();
  // Read Humi
  float h = dht.readHumidity();
  // Check isRead ?
  if (isnan(h) || isnan(t)) {
    delay(500);
    Serial.println("Failed to read from DHT sensor!\n");
    return;
  }

  Blynk.virtualWrite(V5, t);
  Blynk.virtualWrite(V4, h);

  Serial.print("\n");
  Serial.print("Humidity: " + String(h) + "%");
  Serial.print("\t");
  Serial.print("Temperature:" + String(t) + " C");
  delay(2000);
}