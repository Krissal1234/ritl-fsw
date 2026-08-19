// ======================================================================
// \title  RecoveryController.hpp
// \author krissal1234
// \brief  hpp file for RecoveryController component implementation class
// ======================================================================

#ifndef RitlFsw_RecoveryController_HPP
#define RitlFsw_RecoveryController_HPP

#include "RitlFsw/Components/Controllers/RecoveryController/RecoveryControllerComponentAc.hpp"

namespace RitlFsw {

class RecoveryController final : public RecoveryControllerComponentBase {
  public:
    // ----------------------------------------------------------------------
    // Component construction and destruction
    // ----------------------------------------------------------------------

    //! Construct RecoveryController object
    RecoveryController(const char* const compName  //!< The component name
    );

    //! Destroy RecoveryController object
    ~RecoveryController();

  private:
    bool m_drogue_fired;
    bool m_main_fired;


    void fireDrogue();
    void fireMain();
    // ----------------------------------------------------------------------
    // Handler implementations for typed input ports
    // ----------------------------------------------------------------------

    //! Handler implementation for baroDataIn
    void baroDataIn_handler(FwIndexType portNum,  //!< The port number
                            F64 data) override;
};

}  // namespace RitlFsw

#endif
