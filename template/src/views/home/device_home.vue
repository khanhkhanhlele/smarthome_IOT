<script setup>
import { onMounted, ref } from 'vue';
import { useRouter } from 'vue-router';
import { getDeviceByRoom, updateDevice } from './../../api/device/index';

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
const confirmDevice = (type, value) => {
    if (type == 'humidity_sensors') {
        if (value > 90) return 'Warning';
        else return 'Ổn định';
    } else if (type == 'LED') {
        return '';
    } else if (type == 'temperature_sensors') {
        if (value > 70) return 'Warning';
        else return 'Ổn định';
    }
};
const handleLed = async (id, status) => {
    const sta = status == 'ON' ? 'OFF' : 'ON';
    listDevice.value.leds.forEach((device) => {
        if (device._id == id) device.status = sta;
    });
    await updateDevice(id, { status: sta });
};
const updateList = (data) => {
    console.log(data);
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
    const nameDevice = listDevice.value.filter((e) => e._id == deviceId)[0].deviceName;
    lineData.value.datasets = lineData.value.datasets.filter((e) => e.label != nameDevice);
    listDevice.value.sensors = listDevice.value.filter((e) => e._id != deviceId);
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
                <div class="card mb-0 surface-200">
                    <div class="flex justify-content-between mb-3">
                        <div>
                            <span class="block text-500 font-medium mb-3">{{ device.deviceName }}</span>
                            <div class="text-900 font-medium text-xl">{{ device.value }}</div>
                        </div>
                        <div class="flex flex-column">
                            <div class="flex align-items-center justify-content-center bg-blue-100 border-round" style="width: 2.5rem; height: 2.5rem">
                                <i class="pi pi-home text-blue-500 text-xl"></i>
                            </div>
                            <Delete_device @update-list="updateListAfterDelete" :device="device"></Delete_device>
                            <Update_device @update-list="updateListAfterUpdate" :device="device"></Update_device>
                        </div>
                    </div>
                    <span class="text-green-500 font-medium">{{ confirmDevice(device.deviceType.name, device.value) }} </span>
                </div>
            </div>
            <div v-for="device in listDevice" :key="device" class="col-12 lg:col-6 xl:col-3">
                <div v-if="device.devic
                eType.name == 'LED'" class="card mb-0" @click="handleLed(device._id, device.status)">
                    <div class="flex justify-content-between mb-3">
                        <div>
                            <span class="block text-500 font-medium mb-3">{{ device.deviceType.description }}</span>
                            <div class="text-900 font-medium text-xl">{{ device.value }}</div>
                        </div>
                        <div class="flex flex-column">
                            <div v-if="device.status == 'ON'" class="flex align-items-center justify-content-center border-round" style="width: 2.5rem; height: 2.5rem">
                                <i class="pi pi-sun text-blue-500 text-xl"></i>
                            </div>
                            <div v-if="device.status == 'OFF'" class="flex align-items-center justify-content-center bg-blue-100 border-round" style="width: 2.5rem; height: 2.5rem">
                                <i class="pi pi-sun text-blue-500 text-xl"></i>
                            </div>
                            <Delete_device @update-list="updateListAfterDelete" :device="device"></Delete_device>
                        </div>
                    </div>
                    <span class="text-green-500 font-medium">{{ device.status }} </span>
                </div>
            </div>
        </div>
        <Chart type="line" :data="lineData" :options="lineOptions" />
    </div>
</template>

<style lang="scss" scoped></style>
