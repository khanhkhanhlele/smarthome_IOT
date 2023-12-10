<script setup>
import { ref, defineProps, defineEmits } from 'vue';
import { useToast } from 'primevue/usetoast';
import { createDevice, getDeviceType } from '../../../api/device';
const toast = useToast();
const visible = ref(false);
const deviceTypes = ref();
const deviceTypeNames = ref();
const form = ref({
    roomId: '',
    deviceTypeId: '',
    deviceName: ''
});
const emit = defineEmits(['update-list']);
const show = async () => {
    visible.value = true;
    const res = await getDeviceType();
    deviceTypes.value = res.result.map((e) => ({
        name: e.name,
        id: e._id
    }));
    console.log(deviceTypes.value);
};
const submit = async () => {
    form.value.roomId = props.roomId;
    console.log(deviceTypeNames.value);
    form.value.deviceTypeId = deviceTypeNames.value.id;
    const res = await createDevice(form.value);
    if (res.device) toast.add({ severity: 'success', summary: 'Success', detail: 'Created room sucessfully', life: 3000 });
    console.log(res);
    emit('update-list', res);
    visible.value = false;
};
const props = defineProps({
    roomId: {
        type: Number
    }
});
</script>
<template>
    <div>
        <Button label="Create" icon="pi pi-external-link" @click="show" />
        <Toast />
        <Dialog v-model:visible="visible" modal header="Tạo phòng mới" :style="{ width: '50rem' }" :breakpoints="{ '1199px': '75vw', '575px': '90vw' }">
            <div class="flex flex-column gap-2">
                <label for="username">Device name</label>
                <InputText id="username" v-model="form.deviceName" aria-describedby="username-help" />
                <label for="username">Device type</label>
                <small id="username-help">Enter device name.</small>
                <Dropdown v-model="deviceTypeNames" :options="deviceTypes" optionLabel="name" placeholder="Select a device type" class="w-full" />
                <Button type="submit" @click="submit" label="Create" />
            </div>
        </Dialog>
    </div>
</template>

<style lang="scss" scoped></style>
