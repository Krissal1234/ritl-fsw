// ======================================================================
// \title  OrchReceiver.cpp
// \author krissal1234
// \brief  cpp file for OrchReceiver component implementation class
// ======================================================================

#include "RitlFsw/Components/SimBridge/OrchReceiver/OrchReceiver.hpp"
#include "Fw/Logger/Logger.hpp"
namespace RitlFsw {

// ----------------------------------------------------------------------
// Component construction and destruction
// ----------------------------------------------------------------------

OrchReceiver ::OrchReceiver(const char* const compName) : OrchReceiverComponentBase(compName) {}

OrchReceiver ::~OrchReceiver() {}


void OrchReceiver ::DataIn_handler(FwIndexType portNum, Fw::Buffer& buffer, const Drv::ByteStreamStatus& status) {

    if (status != Drv::ByteStreamStatus::OP_OK) {
        deallocate_out(0, buffer);
        return;
    }

    U8 ackByte = 0x06;
    Fw::Buffer ackBuf(&ackByte, 1);
    DataOut_out(0, ackBuf);

    Fw::ExternalSerializeBuffer extBuf(
        buffer.getData(),
        buffer.getSize()
    );
    extBuf.setBuffLen(buffer.getSize());

    RitlFsw::SensorData sensorData;
    Fw::SerializeStatus serStatus = sensorData.deserializeFrom(extBuf);

    if (serStatus == Fw::FW_SERIALIZE_OK) {
        sensorDataOut_out(0, sensorData);
    } else {
        Fw::Logger::log("serSTatus failed");
    }

    deallocate_out(0, buffer);
}

}  // namespace RitlFsw
