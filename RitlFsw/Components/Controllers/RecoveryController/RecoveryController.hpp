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
    F64  m_min_baro;
    I32  m_baro_count;
    bool m_drogue_fired;
    bool m_main_fired;
    F64  m_prev_baro;
    I32  m_baro_frozen_count;
    bool m_baro_frozen;
    F64  m_last_t;
    bool m_launched;
    F64  m_drogue_time;
    static constexpr F64 MAIN_DEPLOY_TIMEOUT = 20.0;

    static constexpr F64 LAUNCH_ACCEL_THRESH   = 20.0;
    static constexpr F64 ACCEL_FREEFALL_THRESH = 2.0;

    static constexpr F64 APOGEE_BARO_DELTA = 0.5;
    static constexpr U32 APOGEE_CONFIRM_COUNT = 5;
    static constexpr F64 MAIN_DEPLOY_DELTA_HPA = 40.0;
    static constexpr F64 BARO_FREEZE_DELTA  = 1;
    static constexpr I32 BARO_FREEZE_COUNT  = 15;


    void fireDrogue();
    void fireMain();
    // ----------------------------------------------------------------------
    // Handler implementations for typed input ports
    // ----------------------------------------------------------------------

    //! Handler implementation for baroDataIn
    void baroDataIn_handler(FwIndexType portNum,  //!< The port number
                            F64 data) override;

    void sensorDataIn_handler(FwIndexType portNum,  //!< The port number
                              const RitlFsw::SensorData& data) override;
};

}  // namespace RitlFsw

#endif
