module RitlFsw {
    passive component ImuSimSensor {

        sync input port imuSensorDataIn: RitlImuData

        guarded input port getImuData : GetImuData

        @ Port for requesting the current time
        time get port timeCaller
    }
}