import { request } from '../../utils/request';

export function getRoom() {
    return request({
        url: 'api/room',
        method: 'get'
    });
}
export function createRoom(data) {
    return request({
        url: 'api/room',
        method: 'post',
        data
    });
}
export function updateRoom(data) {
    return request({
        url: 'api/room',
        method: 'put',
        data
    });
}
export function deleteRoom(data) {
    return request({
        url: 'api/room',
        method: 'delete',
        data
    });
}
