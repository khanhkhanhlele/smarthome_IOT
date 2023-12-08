const router = require('express').Router();

const { getById, getByRoom, addToRoom, destroy,update } = require('../controllers/device.controller');

router.get("/room/:roomId", getByRoom);
router.get("/:deviceId", getById);
router.post("/room", addToRoom);
router.delete("/:deviceId", destroy);
router.put("/:deviceId", update);

module.exports = router;