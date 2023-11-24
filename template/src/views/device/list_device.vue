<script setup>
import { onMounted, ref } from 'vue';
import { useRouter } from 'vue-router';
import { getDeviceByRoom } from './../../api/device/index';
import create_device from './create_device.vue';
const router = useRouter();
console.log(router.params);
const listDevice = ref();
onMounted(async () => {
    listDevice.value = await getDeviceByRoom(router.currentRoute.value.params.id);
});
</script>
<template>
    <div>
        {{ listDevice }}
        <create_device :roomId="router.currentRoute.value.params.id"></create_device>
    </div>
</template>

<style lang="scss" scoped></style>
