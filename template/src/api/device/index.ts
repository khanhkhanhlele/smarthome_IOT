import { request } from '../../utils/request';

export function getDeviceByRoom(roomId) {
    return request({
        url: `api/device/room/${roomId}`,
        method: 'get'
    });
}
export function getDeviceByDevice(deviceId) {
    return request({
        url: `api/device/${deviceId}`,
        method: 'get'
    });
}
export function createDevice(data) {
    return request({
        url: 'api/device/room',
        method: 'post',
        data
    });
}
export function deleteRoom(deviceId) {
    return request({
        url: `api/device/room/${deviceId}`,
        method: 'delete'
    });
}
export function updateDevice(deviceId, data) {
    return request({
        url: `api/device/${deviceId}`,
        method: 'put',
        data
    });
}
export function deleteDevice(deviceId) {
    return request({
        url: `api/device/${deviceId}`,
        method: 'delete',
    });
}
export function getDeviceType() {
    return request({
        url: `api/deviceType`,
        method: 'get'
    });
}