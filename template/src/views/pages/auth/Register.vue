<script setup>
import { useLayout } from '@/layout/composables/layout';
import { ref, computed } from 'vue';
import to from './../../../utils/awaitTo';
import { useRouter } from 'vue-router';
import { useToast } from 'primevue/usetoast';
import { register } from './../../../api/user/index';

const toast = useToast();
const { layoutConfig } = useLayout();
const email = ref('');
const name = ref('');
const password = ref('');
const confirmPassword = ref('');
const router = useRouter();

const registerFunc = async () => {
  // Thực hiện các bước để đăng ký tài khoản
  if(password.value !== confirmPassword.value) {
    toast.add({ severity: 'error', summary: 'Error', detail: 'Password and confirm password not match', life: 3000 });
  };
  try{
    console.log(
      {name: name.value, email: email.value, password: password.value }
    );
    const res = await register({name: name.value, email: email.value, password: password.value });
    console.log(res);
    if(res.token) {
      toast.add({ severity: 'success', summary: 'Success', detail: 'Register success', life: 3000 });
      //wait 1s
      setTimeout(() => {
        router.push('/auth/login');
      }, 1000);
    }
  } catch (error) {
    console.error(error);
    toast.add({ severity: 'error', summary: 'Error', detail: 'Register failed', life: 3000 });
  }
  
};

const logoUrl = computed(() => {
  return `layout/images/${layoutConfig.darkTheme.value ? 'logo-white' : 'logo-dark'}.svg`;
});
</script>


<template>
  <Toast />
    <div class="surface-ground flex align-items-center justify-content-center min-h-screen min-w-screen overflow-hidden">
      <div class="flex flex-column align-items-center justify-content-center">
        <img :src="logoUrl" alt="Sakai logo" class="mb-5 w-6rem flex-shrink-0" />
        <div style="border-radius: 56px; padding: 0.3rem; background: linear-gradient(180deg, var(--primary-color) 10%, rgba(33, 150, 243, 0) 30%)">
          <div class="w-full surface-card py-8 px-5 sm:px-8" style="border-radius: 53px">
            <div class="text-center mb-5">
              <img src="/demo/images/login/avatar.png" alt="Image" height="50" class="mb-3" />
              <div class="text-900 text-3xl font-medium mb-3">Create an Account</div>
              <span class="text-600 font-medium">Sign up to get started</span>
            </div>
  
            <div>
              <label for="username1" class="block text-900 text-xl font-medium mb-2">Username</label>
              <InputText id="username1" type="text" placeholder="Username" class="w-full md:w-30rem mb-5" style="padding: 1rem" v-model="name" />

              <label for="email1" class="block text-900 text-xl font-medium mb-2">Email</label>
              <InputText id="email1" type="text" placeholder="Email address" class="w-full md:w-30rem mb-5" style="padding: 1rem" v-model="email" />
  
              <label for="password1" class="block text-900 font-medium text-xl mb-2">Password</label>
              <Password id="password1" v-model="password" placeholder="Password" :toggleMask="true" class="w-full mb-3" inputClass="w-full" :inputStyle="{ padding: '1rem' }"></Password>
  
              <label for="confirmPassword1" class="block text-900 font-medium text-xl mb-2">Confirm Password</label>
              <Password id="confirmPassword1" v-model="confirmPassword" placeholder="Confirm Password" :toggleMask="true" class="w-full mb-3" inputClass="w-full" :inputStyle="{ padding: '1rem' }"></Password>
  
              <Button label="Register" class="w-full p-3 text-xl mt-3" @click="registerFunc"></Button>
            </div>
          </div>
        </div>
      </div>
    </div>
  </template>

<style scoped>
.pi-eye {
    transform: scale(1.6);
    margin-right: 1rem;
}

.pi-eye-slash {
    transform: scale(1.6);
    margin-right: 1rem;
}
</style>
  
