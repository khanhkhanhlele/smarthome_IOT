<script setup>
import { ref, defineProps, defineEmits } from 'vue';
import { useToast } from 'primevue/usetoast';
const toast = useToast();
const visible = ref(false);
const emit = defineEmits(['update-list']);
const show = async () => {
    visible.value = true;
};

const props = defineProps({
    device: {
        type: Object
    }
});
</script>
<template>
    <div>
        <div @click="show" class="flex align-items-center justify-content-center mt-2 bg-green-100 border-round" style="width: 2.5rem; height: 2.5rem">
            <i class="pi pi-search text-white-500 text-xl"></i>
        </div>
        <Toast />
        <Dialog v-model:visible="visible" modal header="Chi tiết thiết bị" :style="{ width: '50rem' }" :breakpoints="{ '1199px': '75vw', '575px': '90vw' }">
            <div class="flex flex-column gap-2">
                <!-- <label for="username">Device: {{ props.device }}</label> -->
                <label for="username">Device: {{ props.device.deviceName }}</label>
                <label for="username">HomeID: {{ props.device.roomId }}</label>
                <label for="username">DeviceID: {{ props.device._id }}</label>
                <label for="username">Host: c878d64840454cd6a4b648b5a1361691.s1.eu.hivemq.cloud</label>
                <label for="username">Post: 8883</label>
                <label for="username">Topic: IOT_command_topic</label>
                <label for="username">Value: {{ props.device.deviceType.description }}</label>
            </div>
        </Dialog>
    </div>
</template>

<style lang="scss" scoped>


</style>
