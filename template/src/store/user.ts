import { defineStore } from 'pinia';
import { ACCESS_TOKEN_KEY } from '../utils/enum/enum';
import Storage from '../utils/Storage';
import { login, register } from '../api/user/index';
import { store } from '.';

interface UserState {
    token: string;
    userInfo: Partial<API.UserInfo>;
}
export const useUserStore = defineStore({
    id: 'user',
    state: (): UserState => ({
        token: Storage.get(ACCESS_TOKEN_KEY, null),
        userInfo: {}
    }),
    getters: {
        getToken(): string {
            return this.token;
        }
    },
    actions: {
        resetToken() {
            this.token = '';
            this.userInfo = {};
            Storage.clear();
            console.log(Storage.get(ACCESS_TOKEN_KEY, null));
        },
        setToken(token: string) {
            this.token = token ?? '';
            const ex = 7 * 24 * 60 * 60 * 1000;
            Storage.set(ACCESS_TOKEN_KEY, this.token, ex);
        },
        async login(params) {
            try {
                const  {user, token}  = await login(params);
                this.setToken(token);
                Storage.set('INFO_ACCOUNT', data?.user);
            } catch (error) {
                return Promise.reject(error);
            }
        }
    }
});

export function useUserStoreWithOut() {
    return useUserStore(store);
}
