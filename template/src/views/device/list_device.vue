<script setup>
import { onMounted, ref } from 'vue';
import { useRouter } from 'vue-router';
import { getDeviceByRoom, updateDevice } from './../../api/device/index';
import create_device from './crud/create_device.vue';
import Delete_device from './crud/delete_device.vue';
import Update_device from './crud/update_device.vue';
import Detail_device from './crud/detail_device.vue';
import socket from './../../service/websocket';
const router = useRouter();
console.log(router.params);
const listDevice = ref();
function getRandomColor() {
    return `#${Math.floor(Math.random() * 16777215)
        .toString(16)
        .padStart(6, '0')}`;
}
const getRandomArray = (baseNumber) => Array.from({ length: 6 }, () => baseNumber + (Math.random() * 6 - 3));
const lineData = ref({
    labels: ['', '', '', '', '', '', ''],
    datasets: []
});
onMounted(async () => {
    listDevice.value = await getDeviceByRoom(router.currentRoute.value.params.id);
    listDevice.value.forEach((device) => {
        if (device.deviceType._id == '65734c30f92efe7b71369b05') {
            lineData.value.datasets.push({
                label: device.deviceName,
                data: getRandomArray(device.value),
                fill: false,
                backgroundColor: getRandomColor(),
                borderColor: getRandomColor(),
                tension: 0.4
            });
            console.log(lineData.value.datasets);
        }
    });
});

socket.addEventListener('message', (event) => {
    console.log(event.data);
    listDevice.value.forEach((device) => {
        if (device._id == JSON.parse(event.data).deviceId) {
            device.value = JSON.parse(event.data).value;
            updateListWebsocket(device);
        }
        // console.log(JSON.parse(event.data).deviceId);
    });
});

const updateList = (data) => {
    console.log(data.device);
    listDevice.value.push(data.device);
    // eslint-disable-next-line no-constant-condition
    if (data.device.deviceType == '65734c30f92efe7b71369b05') {
        lineData.value.datasets.push({
            label: data.device.deviceName,
            data: getRandomArray(0),
            fill: false,
            backgroundColor: getRandomColor(),
            borderColor: getRandomColor(),
            tension: 0.4
        });
        console.log(lineData.value.datasets);
    }
};
const updateListAfterDelete = (deviceId) => {
    console.log(deviceId);
    const nameDevice = listDevice.value.filter((e) => e._id == deviceId)[0].deviceName;
    lineData.value.datasets = lineData.value.datasets.filter((e) => e.label != nameDevice);
    listDevice.value = listDevice.value.filter((e) => e._id != deviceId);
};

const updateListWebsocket = (device) => {
    // console.log(deviceId);
    listDevice.value.forEach((e) => {
        if (e._id == device._id) {
            e.deviceName = device.deviceName;
            e.value = device.value;
        }
    });
    lineData.value.datasets[0].data = [...lineData.value.datasets[0].data.slice(1), device.value];
};

const updateListAfterUpdate = (device) => {
    // console.log(deviceId);
    listDevice.value.forEach((e) => {
        if (e._id == device._id) {
            e.deviceName = device.deviceName;
            e.value = device.value;
        }
    });
    lineData.value.datasets[0].data = [...lineData.value.datasets[0].data.slice(1), device.value];
};

const lineOptions = ref({
    plugins: {
        legend: {
            labels: {
                color: '#495057'
            }
        }
    },
    scales: {
        x: {
            ticks: {
                color: '#495057'
            },
            grid: {
                color: '#ebedef'
            }
        },
        y: {
            ticks: {
                color: '#495057'
            },
            grid: {
                color: '#ebedef'
            }
        }
    }
});
</script>
<template>
    <div v-if="listDevice">
        <create_device @update-list="updateList" :roomId="router.currentRoute.value.params.id"></create_device>
        <div class="grid">
            <div v-for="device in listDevice" :key="device" class="col-12 lg:col-6 xl:col-3">
                <div class="card mb-0 surface-200 background-image" :style="{ backgroundImage: 'url(./../../../demo/images/deviceType/' + device.deviceType.name + '.png)' }">
                    <div class="flex justify-content-between mb-3">
                        <div >
                            <span class="block text-lg font-bold mb-3 text-xl text-background">{{ device.deviceName }}</span>
                            <span class="block text-500 font-medium mb-3 text-background">{{ device.deviceType.name }}</span>
                            <div class="text-900 font-medium text-xl text-background">{{ device.value }}</div>
                        </div>
                        <div class="flex flex-column">
                            <!-- <div class="flex align-items-center justify-content-center bg-blue-100 border-round" style="width: 2.5rem; height: 2.5rem">
                                <i class="pi pi-home text-blue-500 text-xl"></i>
                            </div> -->
                            <Detail_device  :device="device"></Detail_device>
                            <Delete_device @update-list="updateListAfterDelete" :device="device"></Delete_device>
                            <Update_device @update-list="updateListAfterUpdate" :device="device"></Update_device>
                        </div>
                    </div>
                </div>
            </div>
            
        </div>
        <!-- <Chart type="line" :data="lineData" :options="lineOptions" /> -->
    </div>
</template>

<style lang="scss" scoped>
.background-image {
    // background-image: url('https://primefaces.org/cdn/primevue/images/product/bamboo-watch.jpg');
    background-size: cover; /* Hoặc sử dụng giá trị khác phù hợp */
    background-repeat: no-repeat;
    background-position: center;
}
.text-background {
  background-color: white; /* Nền màu trắng */
  padding: 2px; /* Thêm đệm xung quanh text */
  border-radius: 5px; /* Nếu bạn muốn các góc được bo tròn */
}

</style>
