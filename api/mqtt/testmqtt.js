var mqtt = require('mqtt')

var options = {
    host: 'broker.emqx.io',
    port: 1883,
    protocol: 'mqtt',
    // username: 'lenamkhanh',
    // password: 'Namkhanh2172'
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
    "deviceId": "656706db7d6372d8674850a2",
    "value": "OFF"
};

client.publish('IOT', JSON.stringify(msg));