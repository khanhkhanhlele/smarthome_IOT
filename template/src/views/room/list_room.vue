<script setup>
import { ref, onMounted, defineEmits } from 'vue';
import create_room from './create_room.vue';
import { getRoom, deleteRoom } from '../../api/room';
import { useRouter } from 'vue-router';

const listRoom = ref([]);
const router = useRouter();
const emit = defineEmits(['update-list']);

onMounted(async () => {
  await loadRooms();
});

const loadRooms = async () => {
  try {
    // Lấy danh sách phòng từ API
    const res = await getRoom();
    listRoom.value = res;
  } catch (error) {
    console.error(error);
  }
};

const responsiveOptions = ref([
  {
    breakpoint: '1400px',
    numVisible: 2,
    numScroll: 1,
  },
  {
    breakpoint: '1199px',
    numVisible: 3,
    numScroll: 1,
  },
  {
    breakpoint: '767px',
    numVisible: 2,
    numScroll: 1,
  },
  {
    breakpoint: '575px',
    numVisible: 1,
    numScroll: 1,
  },
]);

const getSeverity = (status) => {
  switch (status) {
    case 'INSTOCK':
      return 'success';
    case 'LOWSTOCK':
      return 'warning';
    case 'OUTOFSTOCK':
      return 'danger';
    default:
      return null;
  }
};

const showDevice = (data) => {
  router.push(`/device/room/${data._id}`);
};

const updateList = (data) => {
  listRoom.value.push(data.result);
};

const deleteRoomHandler = async (roomId) => {
  try {
    const res = await deleteRoom({ roomId });
    console.log(res);

    if (res.message === 'Delete success') {
      // Thông báo xóa thành công
      if (res.result) {
        toast.add({ severity: 'success', summary: 'Success', detail: 'Created room sucessfully', life: 3000 });
      }

      // Cập nhật danh sách phòng sau khi xóa
      listRoom.value = listRoom.value.filter((room) => room._id !== roomId);
      emit('update-list', res);
      await loadRooms();
    }
  } catch (error) {
    console.error(error);
  }
};
</script>

<template>
  <div class="card">
    <create_room @update-list="updateList"></create_room>
    <Carousel :value="listRoom" :numVisible="3" :numScroll="3" :responsiveOptions="responsiveOptions" style="margin-top: 40px">
      <template #item="slotProps">
        <div class="border-1 surface-border border-round m-2 text-center py-5 px-3" @click="showDevice(slotProps.data)">
          <div class="mb-3">
            <img :src="'https://primefaces.org/cdn/primevue/images/product/bamboo-watch.jpg'" :alt="slotProps.data" class="w-6 shadow-2" />
          </div>
          <div>
            <h4 class="mb-1">{{ slotProps.data.name }}</h4>
            <h6 class="mt-0 mb-3">${{ slotProps.data.createdAt }}</h6>
            <Tag :value="slotProps.data.inventoryStatus" :severity="getSeverity(slotProps.data.inventoryStatus)" />
            <div class="mt-5 flex align-items-center justify-content-center gap-2">
              <Button icon="pi pi-search" rounded />
              <Button icon="pi pi-star-fill" rounded severity="secondary" />
              <!-- Thêm nút xóa và gọi hàm xóa khi nút được nhấn -->
              <Button icon="pi pi-trash" @click="deleteRoomHandler(slotProps.data._id)" />
            </div>
          </div>
        </div>
      </template>
    </Carousel>
  </div>
</template>
