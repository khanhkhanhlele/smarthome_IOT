<script setup>
import { ref, defineProps, defineEmits } from 'vue';
import { useToast } from 'primevue/usetoast';
import { deleteDevice } from '../../../api/device';
const toast = useToast();
const visible = ref(false);
const emit = defineEmits(['update-list']);
const show = async () => {
    visible.value = true;
};
const submit = async () => {
    const res = await deleteDevice(props.device._id);
    if (res.device) toast.add({ severity: 'success', summary: 'Success', detail: 'Created room sucessfully', life: 3000 });
    console.log(res);
    emit('update-list', props.device._id);
    visible.value = false;
};
const props = defineProps({
    device: {
        type: Object
    }
});
</script>
<template>
    <div>
        <div @click="show" class="flex align-items-center justify-content-center mt-2 bg-red-100 border-round" style="width: 2.5rem; height: 2.5rem">
            <i class="pi pi-trash text-red-500 text-xl"></i>
        </div>
        <Toast />
        <Dialog v-model:visible="visible" modal header="Xóa thiết bị" :style="{ width: '50rem' }" :breakpoints="{ '1199px': '75vw', '575px': '90vw' }">
            <div class="flex flex-column gap-2">
                <label for="username">Device: {{ props.device.deviceName }}</label>
                <Button type="submit" @click="submit" label="Delete" />
            </div>
        </Dialog>
    </div>
</template>

<style lang="scss" scoped></style>
