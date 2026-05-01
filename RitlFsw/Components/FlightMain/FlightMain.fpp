module RitlFsw {
    @ Main state machine component
    active component FlightMain {

        @ Receive deserialized sensor data from OrchReceiver
        async input port sensorDataIn : RitlSensorData

        @ Send actuation commands to OrchSender
        output port actuationOut : RitlActuationCommand

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