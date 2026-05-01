module RitlFsw {

    @ TCP listener: receives bytes, sends ACK, forwards data out
    passive component OrchReceiver {

        # ------------------------------------------------------------------
        # ByteStreamDriverModel ports
        # ------------------------------------------------------------------

        @ Called by TcpServer when data arrives
        sync input port DataIn : Drv.ByteStreamData

        @ Send bytes back over TCP, used for ACK
        output port DataOut : Drv.ByteStreamSend

        @ Allocate buffer from buffer manager
        output port allocate : Fw.BufferGet

        @ Return buffer to buffer manager
        output port deallocate : Fw.BufferSend

        # ------------------------------------------------------------------
        # Application output
        # ------------------------------------------------------------------

        @ Forward received sensor data downstream
        output port sensorDataOut : RitlSensorData

        # ------------------------------------------------------------------
        # Standard AC ports
        # ------------------------------------------------------------------

        time get port timeCaller
        event port Log
        text event port LogText
        telemetry port Tlm

    }

}