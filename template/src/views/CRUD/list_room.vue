<script setup>
import { ref, onMounted, defineEmits } from 'vue';
import create_room from './create_room.vue';
import Delete_room from './delete_room.vue';
import { getRoom, deleteRoom } from '../../api/room';
import { useRouter } from 'vue-router';
import Toast from 'primevue/toast';

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


const showDevice = (data) => {
  console.log(data);
  router.push(`/device/room/${data._id}`);
};

const updateListAfterCreate = (data) => {
  listRoom.value.push(data.result);
};



const updateListAfterDelete = (roomId) => {
  listRoom.value = listRoom.value.filter((room) => room._id !== roomId);
};

</script>

<template>
  <div class="card">
    <Toast/>
    <create_room @update-list="updateListAfterCreate"></create_room>
    <Carousel :value="listRoom" :numVisible="3" :numScroll="3" :responsiveOptions="responsiveOptions" style="margin-top: 40px">
      <template #item="slotProps">
        <div class="border-1 surface-border border-round m-2 text-center py-5 px-3">
          <div class="mb-3">
            <img :src="'./../../../demo/images/house/house'+ slotProps.index +'.jpg'" :alt="slotProps.data" class="w-10 shadow-2" @click="showDevice(slotProps.data)"/>
          </div>
          <div>
            <h4 class="mb-1">{{ slotProps.data.name }}</h4>
            <!-- <h6 class="mt-0 mb-3">${{ slotProps.data.createdAt }}</h6> -->
            <!-- <Tag :value="slotProps.data.inventoryStatus" :severity="getSeverity(slotProps.data.inventoryStatus)" /> -->
            <div class="mt-5 flex align-items-center justify-content-center gap-2">
              <Button icon="pi pi-search" rounded />
              <Button icon="pi pi-star-fill" rounded severity="secondary" />
              <Delete_room @update-list="updateListAfterDelete" :room ="slotProps.data"></Delete_room>


            </div>
          </div>
        </div>
      </template>
    </Carousel>
  </div>
</template>
