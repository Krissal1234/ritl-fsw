#ifndef RITLFSW_ORCHSENDER_HPP
#define RITLFSW_ORCHSENDER_HPP

#include "RitlFsw/Components/SimBridge/OrchSender/OrchSenderComponentAc.hpp"

namespace RitlFsw {

class OrchSender : public OrchSenderComponentBase {
  public:
    OrchSender(const char* const compName);
    ~OrchSender();

  private:
    U32 m_cmdsSent;
    void sendActuation_handler(FwIndexType portNum,
                               const RitlFsw::ActuationCommand& cmd) override;

    void recvDataIn_handler(FwIndexType portNum,
                            Fw::Buffer& buffer,
                            const Drv::ByteStreamStatus& status) override;

    void sendRaw(const RitlFsw::ActuationCommand& cmd);

};

}  // namespace RitlFsw
#endif