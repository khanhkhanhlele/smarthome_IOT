<script setup>
import { ref, defineProps, defineEmits } from 'vue';
import { useToast } from 'primevue/usetoast';
import { getDeviceType, updateDevice } from '../../../api/device';
const toast = useToast();
const visible = ref(false);
const deviceTypes = ref();
const deviceTypeNames = ref();
const form = ref({
    roomId: '',
    deviceTypeId: '',
    deviceName: '',
    value: ''
});
const emit = defineEmits(['update-list']);
const show = async () => {
    visible.value = true;
    console.log(props.device);
    const res = await getDeviceType();
    deviceTypes.value = res.result.map((e) => ({
        name: e.name,
        id: e._id
    }));
    form.value.deviceTypeId = deviceTypes.value.filter((e) => e.id == props.device._id)[0];
    deviceTypeNames.value = { name: props.device.deviceType.name, id: props.device.deviceType._id };
    form.value.deviceName = props.device.deviceName;
    form.value.roomId = props.device.roomId;
};
const submit = async () => {
    form.value.roomId = props.roomId;
    console.log(deviceTypeNames.value);
    form.value.deviceTypeId = deviceTypeNames.value.id;
    const res = await updateDevice(props.device._id, form.value);
    if (res.result) toast.add({ severity: 'success', summary: 'Success', detail: 'Updated device sucessfully', life: 3000 });
    console.log(res);
    emit('update-list', res.result);
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
        <div @click="show" class="flex align-items-center justify-content-center mt-2 bg-blue-100 border-round" style="width: 2.5rem; height: 2.5rem">
            <i class="pi pi-wrench text-blue-500 text-xl"></i>
        </div>
        <Toast />
        <Dialog v-model:visible="visible" modal header="Cập nhật thông tin thiết bị" :style="{ width: '50rem' }" :breakpoints="{ '1199px': '75vw', '575px': '90vw' }">
            <div class="flex flex-column gap-2">
                <label for="username">Device name</label>
                <InputText id="username" v-model="form.deviceName" aria-describedby="username-help" />
                <label for="username">Value</label>
                <InputText id="username" v-model="form.value" aria-describedby="username-help" />
                <label for="username">Device type</label>
                <Dropdown v-model="deviceTypeNames" :options="deviceTypes" optionLabel="name" placeholder="Select a device type" class="w-full" />
                <Button type="submit" @click="submit" label="Update" />
            </div>
        </Dialog>
    </div>
</template>

<style lang="scss" scoped></style>
