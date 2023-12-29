// Create a WebSocket connection to the backend
const socket = new WebSocket('ws://localhost:8080');

// Connection opened
socket.addEventListener('open', () => {
    console.log('WebSocket is open now.');
});

// // Listen for messages from the server
// socket.addEventListener('message', (event) => {
//     console.log('Message from server: ', event.data);
// });

// Connection closed
socket.addEventListener('close', () => {
    console.log('WebSocket is closed now.');
});

// Connection error
socket.addEventListener('error', (event) => {
    console.error('WebSocket error: ', event);
});
export default socket;