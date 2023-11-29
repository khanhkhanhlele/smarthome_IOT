const mqtt = require("mqtt");
const DeviceData = require('../models/devicedata.model');
const Device = require('../models/device.model');

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
client.subscribe('IOT');

client.on("message", addDatas);



