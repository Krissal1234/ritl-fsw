// ======================================================================
// \title  AirbrakeController.hpp
// \author krissal1234
// \brief  hpp file for AirbrakeController component implementation class
// ======================================================================

#ifndef RitlFsw_AirbrakeController_HPP
#define RitlFsw_AirbrakeController_HPP

#include "RitlFsw/Components/Controllers/AirbrakeController/AirbrakeControllerComponentAc.hpp"

namespace RitlFsw {

class AirbrakeController final : public AirbrakeControllerComponentBase {
  public:
    // ----------------------------------------------------------------------
    // Component construction and destruction
    // ----------------------------------------------------------------------

    //! Construct AirbrakeController object
    AirbrakeController(const char* const compName  //!< The component name
    );

    //! Destroy AirbrakeController object
    ~AirbrakeController();

  private:
    // PI state
    F64  m_integral = 0.0;
    F64  m_vz = 0.0;
    F64  m_prev_alt = 0.0;
    F64  m_P0 = -1.0;   // ground pressure, set on first call
    bool m_burned_out = false;
    U32  m_descent_count = 0;
    bool m_has_data = false;
    F64 m_prev_sim_time_for_dt = -1.0;

    static constexpr F64 TARGET_APOGEE = 3000.0;
    static constexpr F64 Kp = 0.001;
    static constexpr F64 Ki = 0.0001;
    static constexpr F64 BOOST_ACCEL_THRESHOLD = 15.0;
    static constexpr F64 g = 9.81;
    static constexpr U32 DESCENT_COUNT_THRESHOLD = 10;
    static constexpr F64 INTEGRAL_CLAMP = 1000.0;
    static constexpr F64 CALL_RATE_HZ = 10.0;

    bool m_burnout_retract_sent = false;
    bool m_apogee_retract_sent  = false;
    F64 m_prev_sim_time = -1.0;

    void setAirbrake(F64 level);


    // ----------------------------------------------------------------------
    // Handler implementations for typed input ports
    // ----------------------------------------------------------------------

    //! Handler implementation for sensorDataIn
    void sensorDataIn_handler(FwIndexType portNum,  //!< The port number
                              const RitlFsw::SensorData& data) override;
};

}  // namespace RitlFsw

#endif
