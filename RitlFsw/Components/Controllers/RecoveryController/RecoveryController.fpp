module RitlFsw {
    @ Controller that managers rocket's altitude and sends parachute commands when appropriate
    passive component RecoveryController {

        sync input port baroDataIn : BaroDataPort
        sync input port sensorDataIn : SensorDataPort

        output port actuationCommandOut : ActuationCommandPort



        @ Port for requesting the current time
        time get port timeCaller

    }
}