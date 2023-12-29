const mqtt = require("mqtt");
const WebSocket = require('ws');
const DeviceData = require('../models/devicedata.model');
const Device = require('../models/device.model');

const PUBLISH_TOPIC = process.env.PUBLISH_TOPIC;
const host = process.env.HOST;
const mqttPort = process.env.MQTTPORT;
const mqttProtocol = process.env.MQTTPROTOCOL;


// Thông số của MQTT broker
const mqttOptions = {
    host: 'broker.emqx.io',
    port: 1883,
    protocol: 'mqtt',
};

const addOneData = async (data) => {
    try {
        const now = (new Date()).toISOString();
        const name = data.name;
        if (!name) {
            console.log(`${now}: Name of data is missing`);
            return;
        }
    
        let value = data.value;
        if (value == undefined) {
            console.log(`${now}: Value of data is missing`);
            return;
        }
        data["timestamp"] = now;
    
        const result = await DeviceData.create(data);
        if (name == "status" && (data.value == "ON" || data.value == "OFF")) {
            await Device.findByIdAndUpdate(data.deviceId, {
                status: data.value
            });
        } else {
            await Device.findByIdAndUpdate(data.deviceId, {
                value: data.value
            });
        }
        return result;
    } catch (err) {
        console.log(err);
        return null;
    }
}

const addDatas = async (topic, message) => {
    try {
        const msgJson = JSON.parse(message.toString());
        console.log(topic, msgJson);
        if (Array.isArray(msgJson)) {
            for (let data of msgJson) {
                await addOneData(data);
            }
        } else {
            await addOneData(msgJson);
        }
    } catch (err) {
        console.log(err);
    }
}

// Kết nối tới MQTT broker
const client = mqtt.connect(mqttOptions);

// Kết nối tới WebSocket server
const wss = new WebSocket.Server({ port: 8080 });

client.subscribe(PUBLISH_TOPIC);
// client.subscribe(topic2);

client.on('connect', function () {
    console.log('Connected');
});

client.on('error', function (error) {
    console.log(error);
});

client.on("message", addDatas);

// Khi có kết nối từ client WebSocket
wss.on('connection', (ws) => {
    console.log('Client connected to WebSocket');

    // client.on("message", addDatas);
    client.on('message', (topic, message) => {
        // Gửi tin nhắn từ MQTT tới client WebSocket
        ws.send(message.toString());
    });

});
