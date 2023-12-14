<template>
  <div class="grid">
    <div class="mb-4">
      <label for="roomSelect" class="block text-500 font-medium mb-2">Select Room:</label>
      <select v-model="selectedRoom" id="roomSelect" class="form-select w-full rounded-md border-gray-300 focus:border-blue-500 focus:ring focus:ring-blue-200 transition">
        <option v-for="room in listRoom" :key="room._id" :value="room._id">{{ room.name }}</option>
      </select>
    </div>
    <!-- Device cards -->
    <div v-for="device in products" :key="device.name" class="col-12 lg:col-6 xl:col-3">
      <div class="card mb-0">
        <div class="flex justify-content-between mb-3">
          <div>
            <span class="block text-500 font-medium mb-3">{{ device.name }}</span>
            <div class="text-900 font-medium text-xl">{{ device.value }}</div>
          </div>
          <div class="flex flex-column">
            <div class="flex align-items-center justify-content-center bg-blue-100 border-round" style="width: 2.5rem; height: 2.5rem">
              <i class="pi pi-home text-blue-500 text-xl"></i>
            </div>
          </div>
        </div>
        <span class="text-green-500 font-medium">Custom Status</span>
      </div>
    </div>

    <div class="col-12 xl:col-6">
      <!-- Chart displaying information -->
      <div class="card">
        <h5>Device Overview</h5>
        <Chart type="line" :data="lineData" :options="lineOptions" />
      </div>
      
      <!-- Other sections of the dashboard -->
      <!-- ... (Other sections can be adjusted based on your requirements) ... -->
    </div>
  </div>
</template>

<script setup>
import { onMounted, ref } from 'vue';
import { getRoom } from 'C:/Users/hongh/OneDrive/Desktop/New folder/smarthome_IOT/template/src/api/room/index.ts';
const products = ref([]);
const lineData = ref({
  labels: ['January', 'February', 'March', 'April', 'May', 'June', 'July'],
  datasets: [
    {
      label: 'First Dataset',
      data: [65, 59, 80, 81, 56, 55, 40],
      fill: false,
      backgroundColor: '#2f4860',
      borderColor: '#2f4860',
      tension: 0.4
    },
    {
      label: 'Second Dataset',
      data: [28, 48, 40, 19, 86, 27, 90],
      fill: false,
      backgroundColor: '#00bb7e',
      borderColor: '#00bb7e',
      tension: 0.4
    }
  ]
});
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
const listRoom = ref([]);
const selectedRoom = ref(null);

onMounted(async () => {
  await loadData();
  await loadRooms();
});
const loadData = async () => {
  // Fetch data from API or any other data source
  const fakeDeviceData = [
    { name: 'Device A', value: 65 },
    { name: 'Device B', value: 30 },
    { name: 'Device C', value: 30 },
    { name: 'Device D', value: 30 },
    // ... Add more devices here
  ];
  products.value = fakeDeviceData;
};
const loadRooms = async () => {
  try {
    // Lấy danh sách phòng từ API
    const res = await getRoom();
    listRoom.value = res;

    // Lựa chọn phòng đầu tiên theo danh sách
    if (listRoom.value.length > 0) {
      selectedRoom.value = listRoom.value[0]._id;
    }
  } catch (error) {
    console.error(error);
  }
};
</script>
