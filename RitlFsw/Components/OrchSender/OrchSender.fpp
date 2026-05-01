module RitlFsw {
    @ Tcp Client to send actuation commands to Orchestrator
    passive component OrchSender {

        sync input port sendActuation: RitlActuationCommand

        @ Sink for orchSenderClient recv events (disconnect, etc.) — buffer is discarded
        sync input port recvDataIn: Drv.ByteStreamData

        output port dataOut: Drv.ByteStreamSend

        output port allocate:   Fw.BufferGet
        output port deallocate: Fw.BufferSend

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
        @ Port to set the value of a parameter
        param set port prmSetOut

        @ Last command ID sent to Python
        telemetry LastCmdSent: U8

        @ Total actuation commands sent
        telemetry CmdsSent: U32

        @ Drogue fire command sent
        event DrogueSent() severity activity high format "Drogue fire command sent"

        @ Main fire command sent
        event MainSent() severity activity high format "Main fire command sent"

        @ Command serialization failed
        event SerializeFailed(cmdId: U8) severity warning high format "Serialize failed for cmd {}"

        @ Buffer allocation failed
        event AllocFailed() severity warning high format "Buffer allocation failed"
    }
}