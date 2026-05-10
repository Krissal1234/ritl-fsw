module RitlFsw {
    @ SensorHub that receives sensordata packet and distributes to SimSensors
    passive component SensorHub {

        @ Receive deserialized sensor data from OrchReceiver
        sync input port sensorDataIn : SensorDataPort

        @ Future proof hook in case testing requires bypassing SimSensors
        guarded input port getSensorData: GetSensorData

        guarded input port getSimReady : GetSimReady

        output port sensorDataOut : SensorDataPort

        @ Port for requesting the current time
        time get port timeCaller
    }
}