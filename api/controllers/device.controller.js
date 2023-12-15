const Device = require('../models/device.model');
const Room = require('../models/room.model');
const DeviceType = require('../models/devicetype.model');
const { NotFoundError, BadRequestError } = require('../errors');
const { StatusCodes } = require('http-status-codes');

const getById = async (req, res) => {
    try {
        const deviceId = req.params.deviceId;
        const device = await Device.findById(deviceId).populate("deviceType");
        if (!device) {
            throw new NotFoundError("Device not found");
        }

        return res.status(StatusCodes.OK).json({ device });
    } catch (err) {
            return res.status(400).json({"err": err.toString()});
    }
}

const getByRoom = async (req, res) => {
    try {
        const roomId = req.params.roomId;
        const room = await Room.exists({ _id: roomId });
        if (!room) {
            throw new NotFoundError("Room not found");
        }

        const devices = await Device.find({
            roomId: roomId
        }).populate("deviceType");

        const sensors = [];
        const leds = [];
        const humidities = [];
        const temperatures = [];
        const linecharts = [];
        for (let Device of devices) {
            if (Device["deviceType"]["name"] === "LED") {
                leds.push(Device);
            } 
            else if (Device["deviceType"]["name"] === "Temperature") {
                temperatures.push(Device);
            }
            else if (Device["deviceType"]["name"] === "Humidity") {
                humidities.push(Device);
            }
            else if (Device["deviceType"]["name"] === "LineChart") {
                linecharts.push(Device);
            }
            else {
                sensors.push(Device);
            }
        }

        return res.status(StatusCodes.OK).json({ sensors, leds , humidities, temperatures, linecharts});
    } catch (err) {
        return res.status(400).json({"err": err.toString()});
    }
}

const addToRoom = async (req, res) => {
    try {
        const roomId = req.body.roomId;
        const deviceTypeId = req.body.deviceTypeId;
        const deviceName = req.body.deviceName;
        console.log(req.body);
        if (!deviceName) {
            throw new NotFoundError("Device name not found");
        }
        const room = await Room.exists({ _id: roomId });
        if (!room) {
            throw new NotFoundError("Room not found");
        }

        if (!(await DeviceType.exists({ _id: deviceTypeId }))) {
            throw new NotFoundError("Device type not found");
        }

        const device = await Device.create({
            roomId,
            deviceType: deviceTypeId,
            deviceName: deviceName
        });
        return res.status(StatusCodes.CREATED).json({ device });
    } catch (err) {
        return res.status(400).json({"err": err.toString()});
    }
}

const update = async (req, res) => {
    try {
        const deviceId = req.params.deviceId;
        const result = await Device.findByIdAndUpdate(deviceId, req.body, {
            new: true,
            runValidators: true
        });

        if (!result) {
            throw new NotFoundError("Device not found");
        }
        // thêm đoạn code thay đổi status đến thiết bị

        return res.status(StatusCodes.OK).json({ result });
    } catch (err) {
        return res.status(400).json({"err": err.toString()});
    }
}


const destroy = async (req, res) => {
    try {
        const deviceId = req.params.deviceId;
        const result = await Device.findByIdAndRemove(deviceId);
        if (!result) {
            throw new NotFoundError("Device not found");
        }

        return res.status(StatusCodes.NO_CONTENT).send();
    } catch (err) {
        return res.status(400).json({"err": err.toString()});
    }
}


module.exports = {
    getById,
    getByRoom,
    addToRoom,
    update,
    destroy
}