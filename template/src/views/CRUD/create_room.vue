<script setup>
import { ref,defineEmits } from 'vue';
import { createRoom } from './../../api/room/index';
import { useToast } from 'primevue/usetoast';
const toast = useToast();
const visible = ref(false);
const name = ref();
const emit = defineEmits(['update-list']);
const submit = async () => {
    const res = await createRoom({ name: name.value });
    console.log(res);
    if (res.result) toast.add({ severity: 'success', summary: 'Success', detail: 'Created room sucessfully', life: 3000 });
    visible.value = false;
    emit('update-list', res);
    console.log(res);
};

</script>
<template>
    <div>
        <Button label="Create" icon="pi pi-external-link" @click="visible = true" />
        <Toast />
        <Dialog v-model:visible="visible" modal header="Tạo nhà  mới" :style="{ width: '50rem' }" :breakpoints="{ '1199px': '75vw', '575px': '90vw' }">
            <div class="flex flex-column gap-2">
                <label for="username">House name</label>
                <InputText id="username" v-model="name" aria-describedby="username-help" />
                <small id="username-help">Enter house name.</small>
                <Button type="submit" @click="submit" label="Create" />
            </div>
        </Dialog>
    </div>
</template>

<style lang="scss" scoped></style>
