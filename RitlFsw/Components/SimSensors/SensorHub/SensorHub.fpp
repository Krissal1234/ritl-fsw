module RitlFsw {
    @ SensorHub that receives sensordata packet and distributes to SimSensors
    passive component SensorHub {

        @ Receive deserialized sensor data from OrchReceiver
        sync input port sensorDataIn : SensorDataPort

        @ Future proof hook in case testing requires bypassing SimSensors
        guarded input port getSensorData: GetSensorData

    @ Port to indicate that Sensor Data is flowing from simulation
        guarded input port getSimReady : GetSimReady

       @ Output ports to individual sensor components
        output port imuDataOut  : ImuDataPort
        output port baroDataOut : BaroDataPort

        @ Port for requesting the current time
        time get port timeCaller
    }
}