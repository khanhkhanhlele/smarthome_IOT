<script setup>
import { ref, computed, onMounted, onBeforeUnmount } from 'vue';
import { useLayout } from '@/layout/composables/layout';
import { useRouter } from 'vue-router';
import { useUserStore } from './../store/user';
import Storage from '../utils/Storage';
const { layoutConfig, onMenuToggle } = useLayout();

const outsideClickListener = ref(null);
const topbarMenuActive = ref(false);
const router = useRouter();

onMounted(() => {
    bindOutsideClickListener();
});

onBeforeUnmount(() => {
    unbindOutsideClickListener();
});

const logoUrl = computed(() => {
    return `layout/images/${layoutConfig.darkTheme.value ? 'logo-white' : 'logo-dark'}.svg`;
});

const onTopBarMenuButton = () => {
    topbarMenuActive.value = !topbarMenuActive.value;
};
const onLogoutClick = async () => {
    useUserStore().resetToken();
    router.push('/landing');
};
const onSettingsClick = () => {
    topbarMenuActive.value = false;
    router.push('/documentation');
};
const topbarMenuClasses = computed(() => {
    return {
        'layout-topbar-menu-mobile-active': topbarMenuActive.value
    };
});

const bindOutsideClickListener = () => {
    if (!outsideClickListener.value) {
        outsideClickListener.value = (event) => {
            if (isOutsideClicked(event)) {
                topbarMenuActive.value = false;
            }
        };
        document.addEventListener('click', outsideClickListener.value);
    }
};
const unbindOutsideClickListener = () => {
    if (outsideClickListener.value) {
        document.removeEventListener('click', outsideClickListener);
        outsideClickListener.value = null;
    }
};
const isOutsideClicked = (event) => {
    if (!topbarMenuActive.value) return;

    const sidebarEl = document.querySelector('.layout-topbar-menu');
    const topbarEl = document.querySelector('.layout-topbar-menu-button');

    return !(sidebarEl.isSameNode(event.target) || sidebarEl.contains(event.target) || topbarEl.isSameNode(event.target) || topbarEl.contains(event.target));
};
const menu = ref();
const items = ref([
    {
        label: 'Profile',
        icon: 'pi pi-user',
        command: () => {}
    },
    {
        label: 'Log out',
        icon: 'pi pi-fw pi-sign-out',
        command: () => {
            useUserStore().resetToken();
            router.push('/landing');
        }
    }
]);

const toggle = (event) => {
    menu.value.toggle(event);
};


const userData = Storage.get('INFO_ACCOUNT', null);
// const userName = userData.name;
</script>

<template>
    <div class="layout-topbar">
        <button class="p-link layout-menu-button layout-topbar-button" style="margin: unset" @click="onMenuToggle()">
            <i class="pi pi-bars"></i>
        </button>
        <router-link to="/" class="layout-topbar-logo">
            <img :src="logoUrl" alt="logo" />
            <span>SmartHome_IOT</span>
        </router-link>

        <button class="p-link layout-topbar-menu-button layout-topbar-button" @click="onTopBarMenuButton()">
            <i class="pi pi-ellipsis-v"></i>
        </button>
        <div class="layout-topbar-menu" :class="topbarMenuClasses">
            <Button class="w-full p-link flex align-items-center p-2 pl-3 text-color hover:surface-200 border-noround" @click="toggle" aria-haspopup="true" aria-controls="overlay_menu">
                <Avatar icon="pi pi-user" class="mr-2" size="large" style="background-color: #2196f3; color: #ffffff" shape="circle" />
                <div class="flex flex-column align">
                    <span class="font-bold">{{ userName }}</span>
                </div>
            </Button>
            <Menu ref="menu" id="overlay_menu" :model="items" :popup="true" />
        </div>
    </div>
</template>

<style lang="scss" scoped>
.dropdown-item {
    background: none;
    border: none;
    cursor: pointer;
    padding: 0.5rem 1rem;
    display: flex;
    align-items: center;
}

.dropdown-item:hover {
    background-color: #f5f5f5;
}

.dropdown .p-dropdown-trigger {
    border: none;
    /* Xóa border xung quanh dropdown */
    box-shadow: none;
    /* Xóa shadow xung quanh dropdown */
}
</style>
