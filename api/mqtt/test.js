const WebSocket = require('ws');
const mqtt = require('mqtt');

// Thông số của MQTT broker
const mqttOptions = {
    host: 'broker.emqx.io',
    port: 1883,
    protocol: 'mqtt',
    // username: 'lenamkhanh',
    // password: 'Namkhanh2172'
};

// Kết nối tới MQTT broker
const mqttClient = mqtt.connect(mqttOptions);

// Kết nối tới WebSocket server
const wss = new WebSocket.Server({ port: 8080 });

// Khi có kết nối từ client WebSocket
wss.on('connection', (ws) => {
    console.log('Client connected to WebSocket');

    // Lắng nghe tin nhắn từ MQTT broker
    mqttClient.subscribe('topic_name'); // Thay 'topic_name' bằng topic MQTT bạn muốn lắng nghe

    mqttClient.on('message', (topic, message) => {
        // Gửi tin nhắn từ MQTT tới client WebSocket
        ws.send(message.toString());
    });

    // Khi client WebSocket đóng kết nối
    ws.on('close', () => {
        console.log('Client disconnected from WebSocket');
        mqttClient.unsubscribe('topic_name'); // Hủy đăng ký lắng nghe khi client đóng kết nối
    });
});
