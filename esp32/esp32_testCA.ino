#include <PubSubClient.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <WiFiServer.h>
#include <WiFiUdp.h>
#include <WiFiClientSecure.h>
#include <ArduinoJson.h>
#include <DHT.h>
#include "ezButton.h"

#define DHTPIN 15
#define DHTTYPE DHT11

WiFiClientSecure wifiClient;
PubSubClient mqttClient(wifiClient); 
char *mqttServer = "79066356b53e43128fe4724754cc9c32.s1.eu.hivemq.cloud";
int mqttPort = 8883;
const char *mqtt_username = "lenamkhanh";
const char *mqtt_password = "Namkhanh2172";
const char *WifiSSID = "Wokwi-GUEST";
const char *WifiPassword = "";
const char *topicSubcribe = "command";
const char *topicPublish = "data";
const char *ledId1 = "640ee113bf49af0a529390ad";
const char *ledId2 = "640ee119bf49af0a529390b1";
const char *tempId = "640ee2c0246bf48329d6deb1";
const char *humiId = "640ee2b1246bf48329d6dead";
const char *homeId = "62ce96a2cd95012e5f7155de";
const char *roomId1 = "62ce96c7cd95012e5f7155e1";
const char *roomId2 = "62ce96cfcd95012e5f7155e4";

StaticJsonDocument<256> sendingStatusLed;
StaticJsonDocument<256> sendingValueSensor;

volatile byte state1 = HIGH;
volatile byte state2 = HIGH;
int LED1 = 5;
int LED2 = 23;

ezButton button1(32);
ezButton button2(33);
DHT dht(DHTPIN, DHTTYPE);
unsigned long start = 0, delayTime = 200000;


void setup() {
  Serial.begin(9600);
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);

  button1.setDebounceTime(50);
  button2.setDebounceTime(50);
  digitalWrite(LED1, state1);
  digitalWrite(LED2, state2);

  dht.begin();
  sendingStatusLed["homeId"] = homeId;
  sendingValueSensor["homeId"] = homeId;
  sendingValueSensor["roomId"] = roomId1;
  
  // Connect to Wi-Fi network
  delay(10);
  WiFi.begin(WifiSSID, WifiPassword);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void loop(){
  if (!mqttClient.connected()) {    
    mqttClient.setServer(mqttServer, mqttPort);
    // set the callback function
    mqttClient.setCallback(callback);
    Serial.println("Connecting to MQTT Broker...");
    while (!mqttClient.connected()) {
      Serial.println("Reconnecting to MQTT Broker..");
      String clientId = "ESP32Client-";
      clientId += String(random(0xffff), HEX);

      if (mqttClient.connect(clientId.c_str(), mqtt_username, mqtt_password)) {
         Serial.println("Connected to MQTT BRoker!.");
         // subscribe to topic
         mqttClient.subscribe(topicSubcribe);
      } else {
          Serial.print("failed with state ");
          Serial.print(mqttClient.state());
          delay(2000);
      }
     }
   }
   mqttClient.loop();

  button1.loop();
  button2.loop();
  if (button1.isPressed())
  {
    state1 = !state1;
    digitalWrite(LED1, state1);
    sendingStatusLed["roomId"] = roomId1;
    sendingStatusLed["name"] = "status";
    sendingStatusLed["value"] = state1 ? "ON" : "OFF";
    sendingStatusLed["deviceId"] = ledId1;
    char out[256];
    serializeJson(sendingStatusLed, out);
    mqttClient.publish(topicPublish, out, true);
    Serial.print("Pin status (LED1): ");
    Serial.println(state1);
  }
  if (button2.isPressed())
  {
    state2 = !state2;
    digitalWrite(LED2, state2);
    sendingStatusLed["roomId"] = roomId2;
    sendingStatusLed["name"] = "status";
    sendingStatusLed["value"] = state2 ? "ON" : "OFF";
    sendingStatusLed["deviceId"] = ledId2;
    char out[256];
    serializeJson(sendingStatusLed, out);
    mqttClient.publish(topicPublish, out, true);
    Serial.print("Pin status (LED2): ");
    Serial.println(state2);
  }
  if (start < delayTime)
  {
    start++;
  }
  else
  {
    start = 0;
    float humi = dht.readHumidity();
    // read temperature in Celsius
    float tempC = dht.readTemperature();
    // read temperature in Fahrenheit
    float tempF = dht.readTemperature(true);

    // check whether the reading is successful or not
    if (isnan(tempC) || isnan(tempF) || isnan(humi))
    {
      Serial.println("Failed to read from DHT sensor!");
    }
    else
    {
      sendingValueSensor["name"] = "humidity";
      sendingValueSensor["deviceId"] = humiId;
      sendingValueSensor["value"] = humi;
      char outHumi[256];
      serializeJson(sendingValueSensor, outHumi);
      mqttClient.publish(topicPublish, outHumi);
      Serial.print("Humidity: ");
      Serial.print(humi);
      Serial.print("%");
      Serial.print("  |  ");
      char outTemp[256];
      sendingValueSensor["name"] = "temperature";
      sendingValueSensor["deviceId"] = tempId;
      sendingValueSensor["value"] = tempC;
      serializeJson(sendingValueSensor, outTemp);
      mqttClient.publish(topicPublish, outTemp);
      Serial.print("Temperature: ");
      Serial.print(tempC);
      Serial.print("°C  ~  ");
      Serial.print(tempF);
      Serial.println("°F");
    }
  }
}

void callback(char *topic, byte *payload, unsigned int length)
{
  Serial.print("Co tin nhan moi tu topic:");
  Serial.println(topic);
  StaticJsonDocument<256> doc;
  deserializeJson(doc, payload);

  if (doc["value"] == "ON")
  {
    if (strcmp(doc["deviceId"], ledId1) == 0 && state1 == LOW)
    {
      Serial.println("LED1 on");
      digitalWrite(LED1, HIGH);
      state1 = HIGH;
      sendingStatusLed["roomId"] = roomId1;
      sendingStatusLed["name"] = "status";
      sendingStatusLed["value"] = "ON";
      sendingStatusLed["deviceId"] = ledId1;
      char out[256];
      serializeJson(sendingStatusLed, out);
      mqttClient.publish(topicPublish, out, true);
    }
    else if (strcmp(doc["deviceId"], ledId2) == 0 && state2 == LOW)
    {
      Serial.println("LED2 on");
      digitalWrite(LED2, HIGH);
      state2 = HIGH;
      sendingStatusLed["roomId"] = roomId2;
      sendingStatusLed["name"] = "status";
      sendingStatusLed["value"] = "ON";
      sendingStatusLed["deviceId"] = ledId2;
      char out[256];
      serializeJson(sendingStatusLed, out);
      mqttClient.publish(topicPublish, out, true);
    }
  }
  else if (doc["value"] == "OFF")
  {
    if (strcmp(doc["deviceId"], ledId1) == 0 && state1 == HIGH)
    {
      Serial.println("LED off");
      digitalWrite(LED1, LOW);
      state1 = LOW;
      sendingStatusLed["roomId"] = roomId1;
      sendingStatusLed["name"] = "status";
      sendingStatusLed["value"] = "OFF";
      sendingStatusLed["deviceId"] = ledId1;
      char out[256];
      serializeJson(sendingStatusLed, out);
      mqttClient.publish(topicPublish, out, true);
    }
    else if (strcmp(doc["deviceId"], ledId2) == 0 && state2 == HIGH)
    {
      Serial.println("LED off");
      digitalWrite(LED2, LOW);
      state2 = LOW;
      sendingStatusLed["roomId"] = roomId2;
      sendingStatusLed["name"] = "status";
      sendingStatusLed["value"] = "OFF";
      sendingStatusLed["deviceId"] = ledId2;
      char out[256];
      serializeJson(sendingStatusLed, out);
      mqttClient.publish(topicPublish, out, true);
    }
  }
}