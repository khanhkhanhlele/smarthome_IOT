const broker = 'broker.emqx.io';
const port = 1884;
const topicData = 'IOT_data';
const topicCommand = 'IOT_command';
const client_id = 'python-mqtt-1';

const client = new Paho.MQTT.Client(broker, port, client_id);

function onConnect() {
  console.log('Connected to MQTT broker');
}

function onFailure(err) {
  console.error('Failed to connect to MQTT broker:', err);
}

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

function connectToBroker() {
  client.connect({
    onSuccess: onConnect,
    onFailure: onFailure,
    // userName: username,
    // password: password,
    useSSL: true,
  });
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