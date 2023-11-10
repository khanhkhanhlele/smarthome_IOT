var mqtt;
var reconnectTimeout=2000;
var host="test.mosquitto.org";
var port=8080;
// var host="mqtt://broker.hivemq.com";
// var port=1883;

function onConnect(){
 console.log("Connected");
 message = new Paho.MQTT.Message("Hello world");
 message.destinationName="sensor1";
 mqtt.send(message);
 console.log("Success!! Topic: " + message.destinationName + ", Message: " + message  )
}

function MQTTconnect(){
 console.log("connecting to " + host + " " + port);
 mqtt = new Paho.MQTT.Client(host,port,"clientjs");
  var options = {
   timeout: 3,
   onSuccess: onConnect,
  };
 mqtt.connect(options); 
}