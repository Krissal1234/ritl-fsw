// ======================================================================
// \title  FlightMain.hpp
// \author krissal1234
// \brief  hpp file for FlightMain component implementation class
// ======================================================================

#ifndef RitlFsw_FlightMain_HPP
#define RitlFsw_FlightMain_HPP

#include "RitlFsw/Components/FlightMain/FlightMainComponentAc.hpp"

namespace RitlFsw {

class FlightMain final : public FlightMainComponentBase {
  public:
    // ----------------------------------------------------------------------
    // Component construction and destruction
    // ----------------------------------------------------------------------

    FlightMain(const char* const compName  //!< The component name
    );

    ~FlightMain();

  private:
    void sensorDataIn_handler(FwIndexType portNum,
                              const RitlFsw::SensorData& data) override;

    F64  m_min_baro;
    I32  m_baro_count;
    bool m_drogue_fired;
    bool m_main_fired;

  static constexpr F64 APOGEE_BARO_DELTA = 0.5;
  static constexpr U32 APOGEE_CONFIRM_COUNT   = 5;
  static constexpr F64 MAIN_DEPLOY_DELTA_HPA  = 40.0;
};

}  // namespace RitlFsw

#endif
