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

    struct ImuSimData {
        accel: Vec3
        gyro: Vec3
    }


    @ Input port from Simulator for incoming IMU data
    port RitlImuData(data: ImuSimData)

    @ Input port from Simulator for incoming Barometer data
    port RitlBaroData(data: F32)

    @ synchronous get port that returns full sensor packet - used for hook on SensorHub
    port GetSensorData() -> SensorData

    @ synchronous get port that returns imu data - used by ImuSimSensor
    port GetImuData() -> ImuSimData

    @ synchronous get port that returns baro data - used by BaroSimSensor
    port GetBaroData() -> F64

    enum CommandId : U8 {
        DROGUE_FIRE = 0x01
        MAIN_FIRE = 0x02
        AIRBRAKE_SET = 0x03
    }

    struct ActuationCommand {
        cmdId: CommandId
        deployment_level: F32
    }

    port RitlSensorData(data: SensorData)

    port RitlActuationCommand(cmd: ActuationCommand)
}