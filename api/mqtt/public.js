var mqtt = require('mqtt')
const COMMANH_TOPIC = process.env.COMMANH_TOPIC;

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

// subscribe to topic 'my/test/topic'
client.subscribe(COMMANH_TOPIC);

// publish message 'Hello' to topic 'my/test/topic'
// const msg = {
//     "name": "status",
//     "deviceId": "656706db7d6372d8674850a2",
//     "value": "OFF"
// };

const publish = (msg) => {
    client.publish(COMMANH_TOPIC, JSON.stringify(msg));
}
exports.publish = publish;
// client.publish(COMMANH_TOPIC, JSON.stringify(msg));
