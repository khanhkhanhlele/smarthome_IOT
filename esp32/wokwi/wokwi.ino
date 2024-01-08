#include <Adafruit_Sensor.h>
#include <DHT_U.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <ESP32Servo.h> // Include the Servo library
#include <ArduinoJson.h>
#include <FS.h>                   //this needs to be first, or it all crashes and burns...
#include <WiFiManager.h>          //https://github.com/tzapu/WiFiManager

// Defining Pins
#define DHTPIN 5
#define LED 2
#define SERVO_PIN 13 // Servo motor pin

// DHT parameters
#define DHTTYPE    DHT22     // DHT 11
DHT_Unified dht(DHTPIN, DHTTYPE);
uint32_t delayMS;

// Servo motor
Servo servo;


// MQTT Credentials
const char* ssid = "Pixel"; // Setting your AP SSID
const char* password = "12345678"; // Setting your AP PSK
const char* mqttServer = "broker.emqx.io";
const char* clientID = "ujaisldaaasdfgh;laslksdja1"; // Client ID username+0001
const char* public_topic = "IOT_publish_topic"; // Publish topic
const char* command_topic = "IOT_command_topic"; // Command topic

const char* humID = "657c190d9ab424a131ca8626";
const char* temID = "657c19149ab424a131ca862b";
const char* ledID = "657c19009ab424a131ca8621";
const char* servoID = "6592c87f8fb3d324696943c1";
StaticJsonDocument<256> doc;


// Parameters for using non-blocking delay
unsigned long previousMillis = 0;
const long interval = 1000;
String msgStr = "";      // MQTT message buffer
float temp, hum;

// Setting up WiFi and MQTT client
WiFiClient espClient;
PubSubClient client(espClient);

void setup_wifi() {
  // delay(10);
  // WiFi.begin(ssid, password);
  // while (WiFi.status() != WL_CONNECTED) {
  //   delay(500);
  //   Serial.print(".");
  // }
  // Serial.println("");
  // Serial.println("WiFi connected");
  // Serial.println("IP address: ");
  // Serial.println(WiFi.localIP());
  //WiFiManager, Local intialization. Once its business is done, there is no need to keep it around
  WiFiManager wm;
  bool res;
  // res = wm.autoConnect(); // auto generated AP name from chipid
  // res = wm.autoConnect("AutoConnectAP"); // anonymous ap
  res = wm.autoConnect("New device IOT",""); // password protected ap

  if(!res) {
      Serial.println("Failed to connect");
      // ESP.restart();
  } 
  else {
      //if you get here you have connected to the WiFi    
      Serial.println("connected...yeey :)");
  }
}

void reconnect() {
  while (!client.connected()) {
    if (client.connect(clientID)) {
      Serial.println("MQTT connected");
      client.subscribe(command_topic);
      Serial.println("Topic Subscribed");
    }
    else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);  // wait 5sec and retry
    }
  }
}

// Subscribe callback
void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived in topic: ");
  Serial.println(topic);
  StaticJsonDocument<256> doc;
  deserializeJson(doc, payload);

  if ( strcmp(doc["deviceId"], ledID) == 0) {
    if (doc["value"] == "ON") {
      Serial.println("LED");
      digitalWrite(LED, HIGH);
    }
    else {
      digitalWrite(LED, LOW);
    }
  }
  else if ( strcmp(doc["deviceId"], servoID) == 0) {
    if (doc["value"].is<const char*>()) { // Check if the value is a string
      const char* valueStr = doc["value"].as<const char*>(); // Extract the string
      int degree = atoi(valueStr); // Convert the string to an integer
      Serial.print("Moving servo to degree: ");
      Serial.println(degree);
      servo.write(degree); // Move the servo to the specified degree
    } else {
      Serial.println("Error: 'value' is not a string.");
    }
  }

}

void setup() {
  Serial.begin(115200);
  // Initialize device.
  dht.begin();
  // Get temperature sensor details.
  sensor_t sensor;
  dht.temperature().getSensor(&sensor);
  dht.humidity().getSensor(&sensor);
  pinMode(LED, OUTPUT);
  digitalWrite(LED, LOW);

  // Setup servo
  servo.attach(SERVO_PIN, 500, 2400);
  // servo.write(0);

  setup_wifi();
  client.setServer(mqttServer, 1883); // Setting MQTT server
  client.setCallback(callback); // Define function which will be called when a message is received.
}

void loop() {
  if (!client.connected()) { // If client is not connected
    reconnect(); // Try to reconnect
  }
  client.loop();
  unsigned long currentMillis = millis(); // Read current time
  // if (currentMillis - previousMillis >= interval) {
  //   previousMillis = currentMillis;

  //   sensors_event_t event;
  //   dht.temperature().getEvent(&event);
  //   float tem = event.temperature;
  //   dht.humidity().getEvent(&event);
  //   float hum = event.relative_humidity;
  //   // Serial.println(tem,hum);
  //   if (!isnan(tem) && !isnan(hum)) {
      
  //     doc["name"] = "humidity";
  //     doc["value"] = hum; //  humidity
  //     doc["deviceId"] = humID;
  //     char outhum[256];
  //     serializeJson(doc, outhum);
  //     client.publish(public_topic, outhum, true);
  //     Serial.print("PUBLISH JSON: ");
  //     Serial.println(outhum);

  //     doc["name"] = "temperature";
  //     doc["value"] = tem; // temperature 
  //     doc["deviceId"] = temID;
  //     char outtemp[256];
  //     serializeJson(doc, outtemp);
  //     client.publish(public_topic, outtemp, true);
  //     Serial.print("PUBLISH JSON: ");
  //     Serial.println(outtemp);
  //   }
  //   delay(50000);
  // }
}
