const addOneData = async (data) => {
//     try {
//         const now = (new Date()).toISOString();
//         const name = data.name;
//         if (!name) {
//             console.log(`${now}: Name of data is missing`);
//             return;
//         }
    
//         let value = data.value;
//         if (value == undefined) {
//             console.log(`${now}: Value of data is missing`);
//             return;
//         }
//         data["timestamp"] = now;
    
//         const result = await DeviceData.create(data);
//         if (name == "status" && (data.value == "ON" || data.value == "OFF")) {
//             await Device.findByIdAndUpdate(data.deviceId, {
//                 status: data.value
//             });
//         } else {
//             await Device.findByIdAndUpdate(data.deviceId, {
//                 value: data.value
//             });
//         }
//         return result;
//     } catch (err) {
//         console.log(err);
//         return null;
//     }
// }

// const addDatas = async (topic, message) => {
//     try {
//         const msgJson = JSON.parse(message.toString());
//         console.log(topic, msgJson);
//         if (Array.isArray(msgJson)) {
//             for (let data of msgJson) {
//                 await addOneData(data);
//             }
//         } else {
//             await addOneData(msgJson);
//         }
//     } catch (err) {
//         console.log(err);
//     }
// }