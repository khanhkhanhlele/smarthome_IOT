var mqtt = require('mqtt')

var options = {
    host: 'c878d64840454cd6a4b648b5a1361691.s1.eu.hivemq.cloud',
    port: 8883,
    protocol: 'mqtts',
    username: 'lenamkhanh',
    password: 'Namkhanh2172'
}

// initialize the MQTT client
var client = mqtt.connect(options);

// setup the callbacks
client.on('connect', function () {
    console.log('Connected');
});

client.on('error', function (error) {
    console.log(error);
});

client.on('message', function (topic, message) {
    // called each time a message is received
    console.log('Received message:', topic, message.toString());
});

// subscribe to topic 'my/test/topic'
client.subscribe('IOT');

// publish message 'Hello' to topic 'my/test/topic'
const msg = {
    "name": "status",
    "deviceId": "640ecef593e18848065f2cc5",
    "value": "ON"
};

client.publish('IOT', JSON.stringify(msg));