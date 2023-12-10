const host = 'broker.emqx.io';
const port = 1884;
const topicData = 'IOT_data';
const topicCommand = 'IOT_command';
const client_id = 'python-mqtt-1';
const mqtt = require("mqtt");
// const client = new Paho.MQTT.Client(broker, port, client_id);
const client = mqtt.connect({
  host: host,
  port: port,
  protocol: "mqtt",
  // username: username,
  // password: password
  }
);
client.on('connect', function () {
  console.log('Connected');
});

client.on('error', function (error) {
  console.log(error);
});

function onMessageDelivered() {
  console.log('Message delivered');
}

function publishMessage(message) {
  //const message = document.getElementById('message').value;
  const mqttMessage = new Paho.MQTT.Message(message);
  mqttMessage.destinationName = topicCommand;
  mqttMessage.qos = 0;
  mqttMessage.retained = false;
  client.send(mqttMessage);
}



function setupForm() {
  const form = document.querySelector('form');
  const publishBtn = document.getElementById('publish-btn');
  publishBtn.addEventListener('click', publishMessage);
  form.addEventListener('submit', (event) => {
    event.preventDefault();
    publishMessage();
  });
}

connectToBroker();
// setupForm();