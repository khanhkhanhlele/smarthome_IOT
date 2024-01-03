#include <FS.h>                   //this needs to be first, or it all crashes and burns...
#include <SPIFFS.h>
#include <Adafruit_Sensor.h>
#include <DHT_U.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <ESP32Servo.h> // Include the Servo library
#include <ArduinoJson.h>
#include <FS.h>                   //this needs to be first, or it all crashes and burns...
#include <WiFiManager.h>     

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


//define your default values here, if there are different values in config.json, they are overwritten.
char deviceID[40] = "657c19009ab424a131ca8621";
const char* mqttServer = "broker.emqx.io";
const char* clientID = "ujaisldaaasdfgh;laslksdja1"; // Client ID username+0001
const char* public_topic = "IOT_publish_topic"; // Publish topic
const char* command_topic = "IOT_command_topic"; // Command topic

StaticJsonDocument<256> doc;


// Parameters for using non-blocking delay
unsigned long previousMillis = 0;
const long interval = 1000;
String msgStr = "";      // MQTT message buffer
float temp, hum;

// Setting up WiFi and MQTT client
WiFiClient espClient;
PubSubClient client(espClient);

//flag for saving data
bool shouldSaveConfig = true;

//callback notifying us of the need to save config
void saveConfigCallback () {
  Serial.println("Should save config");
  shouldSaveConfig = true;
}

void setup_wifi() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.println();

  //clean FS, for testing
  //SPIFFS.format();

  //read configuration from FS json
  Serial.println("mounting FS...");

  if (SPIFFS.begin()) {
    Serial.println("mounted file system");
    if (SPIFFS.exists("/config.json")) {
      //file exists, reading and loading
      Serial.println("reading config file");
      File configFile = SPIFFS.open("/config.json", "r");
      if (configFile) {
        Serial.println("opened config file");
        size_t size = configFile.size();
        // Allocate a buffer to store contents of the file.
        std::unique_ptr<char[]> buf(new char[size]);

        configFile.readBytes(buf.get(), size);

 #if defined(ARDUINOJSON_VERSION_MAJOR) && ARDUINOJSON_VERSION_MAJOR >= 6
        DynamicJsonDocument json(1024);
        auto deserializeError = deserializeJson(json, buf.get());
        serializeJson(json, Serial);
        if ( ! deserializeError ) {
#else
        DynamicJsonBuffer jsonBuffer;
        JsonObject& json = jsonBuffer.parseObject(buf.get());
        json.printTo(Serial);
        if (json.success()) {
#endif
          Serial.println("\nparsed json");
          // strcpy(mqtt_server, json["mqtt_server"]);
          // strcpy(mqtt_port, json["mqtt_port"]);
          // strcpy(api_token, json["api_token"]);
          strcpy(deviceID, json["deviceID"]);
        } else {
          Serial.println("failed to load json config");
        }
        configFile.close();
      }
    }
  } else {
    Serial.println("failed to mount FS");
  }
  //end read

  // The extra parameters to be configured (can be either global or just in the setup)
  // After connecting, parameter.getValue() will get you the configured value
  // id/name placeholder/prompt default length
  // WiFiManagerParameter custom_mqtt_server("server", "mqtt server", mqtt_server, 40);
  // WiFiManagerParameter custom_mqtt_port("port", "mqtt port", mqtt_port, 6);
  // WiFiManagerParameter custom_api_token("apikey", "API token", api_token, 32);
  WiFiManagerParameter custom_deviceID("deviceID", "deviceID", deviceID, 32);

  //WiFiManager
  //Local intialization. Once its business is done, there is no need to keep it around
  WiFiManager wifiManager;

  //set config save notify callback
  wifiManager.setSaveConfigCallback(saveConfigCallback);

  //set static ip
  // wifiManager.setSTAStaticIPConfig(IPAddress(10, 0, 1, 99), IPAddress(10, 0, 1, 1), IPAddress(255, 255, 255, 0));

  //add all your parameters here
  // wifiManager.addParameter(&custom_mqtt_server);
  // wifiManager.addParameter(&custom_mqtt_port);
  // wifiManager.addParameter(&custom_api_token);
  wifiManager.addParameter(&custom_deviceID);

  //reset settings - for testing
  //wifiManager.resetSettings();

  //set minimu quality of signal so it ignores AP's under that quality
  //defaults to 8%
  //wifiManager.setMinimumSignalQuality();

  //sets timeout until configuration portal gets turned off
  //useful to make it all retry or go to sleep
  //in seconds
  //wifiManager.setTimeout(120);

  //fetches ssid and pass and tries to connect
  //if it does not connect it starts an access point with the specified name
  //here  "AutoConnectAP"
  //and goes into a blocking loop awaiting configuration
  if (!wifiManager.autoConnect("New device IOT", "")) {
    Serial.println("failed to connect and hit timeout");
    delay(3000);
    //reset and try again, or maybe put it to deep sleep
    ESP.restart();
    delay(5000);
  }

  //if you get here you have connected to the WiFi
  Serial.println("connected...yeey :)");

  //read updated parameters
  // strcpy(mqtt_server, custom_mqtt_server.getValue());
  // strcpy(mqtt_port, custom_mqtt_port.getValue());
  // strcpy(api_token, custom_api_token.getValue());
  strcpy(deviceID, custom_deviceID.getValue());
  Serial.println("The values in the file are: ");
  // Serial.println("\tmqtt_server : " + String(mqtt_server));
  // Serial.println("\tmqtt_port : " + String(mqtt_port));
  // Serial.println("\tapi_token : " + String(api_token));
  Serial.println("\tdeviceID : " + String(deviceID));

  //save the custom parameters to FS
  if (shouldSaveConfig) {
    Serial.println("saving config");
 #if defined(ARDUINOJSON_VERSION_MAJOR) && ARDUINOJSON_VERSION_MAJOR >= 6
    DynamicJsonDocument json(1024);
#else
    DynamicJsonBuffer jsonBuffer;
    JsonObject& json = jsonBuffer.createObject();
#endif
    // json["mqtt_server"] = mqtt_server;
    // json["mqtt_port"] = mqtt_port;
    // json["api_token"] = api_token;
    json["deviceID"] = deviceID;

    File configFile = SPIFFS.open("/config.json", "w");
    if (!configFile) {
      Serial.println("failed to open config file for writing");
    }

#if defined(ARDUINOJSON_VERSION_MAJOR) && ARDUINOJSON_VERSION_MAJOR >= 6
    serializeJson(json, Serial);
    serializeJson(json, configFile);
#else
    json.printTo(Serial);
    json.printTo(configFile);
#endif
    configFile.close();
    //end save
  }

  Serial.println("local ip");
  Serial.println(WiFi.localIP());
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

  if ( strcmp(doc["deviceId"], deviceID) == 0) {
    if (doc["value"] == "ON") {
      Serial.println("LED");
      digitalWrite(LED, HIGH);
    }
    else {
      digitalWrite(LED, LOW);
    }
  }
  else if ( strcmp(doc["deviceId"], deviceID) == 0) {
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
  // unsigned long currentMillis = millis(); // Read current time
  // if (currentMillis - previousMillis >= interval) {
  //   previousMillis = currentMillis;

  //   sensors_event_t event;
  //   dht.temperature().getEvent(&event);
  //   float tem = event.temperature;
  //   dht.humidity().getEvent(&event);
  //   float hum = event.relative_humidity;
  //   Serial.println(tem,hum);
  //   if (!isnan(tem) && !isnan(hum)) {
      
  //     doc["name"] = "humidity";
  //     doc["value"] = hum; //  humidity
  //     doc["deviceId"] = deviceID;
  //     char outhum[256];
  //     serializeJson(doc, outhum);
  //     client.publish(public_topic, outhum, true);
  //     Serial.print("PUBLISH JSON: ");
  //     Serial.println(outhum);

  //     doc["name"] = "temperature";
  //     doc["value"] = tem; // temperature 
  //     doc["deviceId"] = deviceID;
  //     char outtemp[256];
  //     serializeJson(doc, outtemp);
  //     client.publish(public_topic, outtemp, true);
  //     Serial.print("PUBLISH JSON: ");
  //     Serial.println(outtemp);
  //   }
  //   delay(50000);
  // }
}
