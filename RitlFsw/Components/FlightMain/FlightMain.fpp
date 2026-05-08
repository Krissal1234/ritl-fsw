module RitlFsw {
    @ Main state machine component
    active component FlightMain {


        @ Scheduler that drives the main component
        async input port run : Svc.Sched

        @ Ports for Polling Sensors - Synchronous Get Pattern
        output port getImu : GetImuData
        output port getBaro : GetBaroData

        output port baroToRecovery : BaroDataPort

        output port sensorDataToControl : SensorDataPort

        output port getSimReady : GetSimReady

        output port getSensorData : GetSensorData




        ###############################################################################
        # Standard AC Ports: Required for Channels, Events, Commands, and Parameters  #
        ###############################################################################

        @ Port for requesting the current time
        time get port timeCaller

        @ Enables command handling
        import Fw.Command

        @ Enables event handling
        import Fw.Event

        @ Enables telemetry channels handling
        import Fw.Channel

        @ Port to return the value of a parameter
        param get port prmGetOut

        @Port to set the value of a parameter
        param set port prmSetOut

    }
}