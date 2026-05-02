module RitlFsw {
    @ Airbrake Controller with PI logic that sends actuation commands to simulator
    passive component AirbrakeController {

        sync input port sensorDataIn: SensorDataPort

        output port actuationCommandOut : ActuationCommandPort

        ###############################################################################
        # Standard AC Ports: Required for Channels, Events, Commands, and Parameters  #
        ###############################################################################
        @ Port for requesting the current time
        time get port timeCaller

        @ Enables command handling
        import Fw.Command

    }
}