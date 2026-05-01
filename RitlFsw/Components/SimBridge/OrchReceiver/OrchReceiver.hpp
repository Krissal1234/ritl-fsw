// ======================================================================
// \title  OrchReceiver.hpp
// \author krissal1234
// \brief  hpp file for OrchReceiver component implementation class
// ======================================================================

#ifndef RitlFsw_OrchReceiver_HPP
#define RitlFsw_OrchReceiver_HPP

#include "RitlFsw/Components/SimBridge/OrchReceiver/OrchReceiverComponentAc.hpp"

namespace RitlFsw {

class OrchReceiver final : public OrchReceiverComponentBase {
  public:
    // ----------------------------------------------------------------------
    // Component construction and destruction
    // ----------------------------------------------------------------------

    //! Construct OrchReceiver object
    OrchReceiver(const char* const compName  //!< The component name
    );

    //! Destroy OrchReceiver object
    ~OrchReceiver();

  private:
    // ----------------------------------------------------------------------
    // Handler implementations for typed input ports
    // ----------------------------------------------------------------------

    void DataIn_handler(FwIndexType portNum,  //!< The port number
                        Fw::Buffer& buffer,
                        const Drv::ByteStreamStatus& status) override;
};

}  // namespace RitlFsw

#endif
