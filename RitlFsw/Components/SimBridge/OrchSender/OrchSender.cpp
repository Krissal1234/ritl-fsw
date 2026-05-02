#include "RitlFsw/Components/SimBridge/OrchSender/OrchSender.hpp"

namespace RitlFsw {

OrchSender::OrchSender(const char* const compName)
    : OrchSenderComponentBase(compName),
      m_cmdsSent(0) {}

OrchSender::~OrchSender() {}

void OrchSender::sendActuation_handler(
    FwIndexType portNum,
    const RitlFsw::ActuationCommand& cmd)
{
    switch (cmd.get_cmdId()) {
        case RitlFsw::CommandId::DROGUE_FIRE:
            this->log_ACTIVITY_HI_DrogueSent();
            break;
        case RitlFsw::CommandId::MAIN_FIRE:
            this->log_ACTIVITY_HI_MainSent();
            break;
        case RitlFsw::CommandId::AIRBRAKE_SET:

            break;
        default:
            return;
    }

    this->sendRaw(cmd);
}


void OrchSender::recvDataIn_handler(FwIndexType portNum,
                                    Fw::Buffer& buffer,
                                    const Drv::ByteStreamStatus& status) {
    // orchSenderClient only sends, any received data discarded
    this->deallocate_out(0, buffer);
}

void OrchSender::sendRaw(const RitlFsw::ActuationCommand& cmd) {

    Fw::Buffer buf = this->allocate_out(0, RitlFsw::ActuationCommand::SERIALIZED_SIZE);
    if (buf.getData() == nullptr) {
        this->log_WARNING_HI_AllocFailed();
        return;
    }

    Fw::ExternalSerializeBuffer extBuf(buf.getData(), buf.getSize());
    Fw::SerializeStatus status = cmd.serializeTo(extBuf);
    if (status != Fw::FW_SERIALIZE_OK) {
        this->log_WARNING_HI_SerializeFailed(
            static_cast<U8>(cmd.get_cmdId())
        );
        this->deallocate_out(0, buf);
        return;
    }

    buf.setSize(extBuf.getSize());

    this->dataOut_out(0, buf);
    this->deallocate_out(0, buf);

    m_cmdsSent++;
    this->tlmWrite_LastCmdSent(static_cast<U8>(cmd.get_cmdId()));
    this->tlmWrite_CmdsSent(m_cmdsSent);
}


}