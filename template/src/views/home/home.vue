<script setup>
import { ref, onMounted, defineEmits } from 'vue';
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
  router.push(`/device_home/${data._id}`);
};

</script>

<template>
  <div class="card">
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
            </div>
          </div>
        </div>
      </template>
    </Carousel>
  </div>
</template>
