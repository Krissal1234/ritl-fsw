module RitlFsw {
    @ Barometer sim sensor that receives pressure from an external simulator
    passive component BaroSimSensor {


        sync input port baroSensorDataIn : RitlBaroData

        guarded input port getBaroData : GetBaroData

        @ Port for requesting the current time
        time get port timeCaller

    }
}