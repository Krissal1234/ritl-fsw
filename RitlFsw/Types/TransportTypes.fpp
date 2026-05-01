module RitlFsw {

    struct Vec3 {
        x: F64
        y: F64
        z: F64
    }

    struct SensorData{
        t:     F64
        accel: Vec3
        baro:  F64
        gyro:  Vec3
    }

    enum CommandId : U8 {
        DROGUE_FIRE = 0x01
        MAIN_FIRE = 0x02
        # AIRBRAKE_SET = 0x03
    }

    struct ActuationCommand {
        cmdId: CommandId
        # deployment_level: F32
    }

    port RitlSensorData(data: SensorData)
    port RitlActuationCommand(cmd: ActuationCommand)
}