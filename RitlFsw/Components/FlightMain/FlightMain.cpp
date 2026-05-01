// ======================================================================
// \title  FlightMain.cpp
// \author krissal1234
// \brief  cpp file for FlightMain component implementation class
// ======================================================================

#include "RitlFsw/Components/FlightMain/FlightMain.hpp"
#include "Fw/Logger/Logger.hpp"

namespace RitlFsw {

// ----------------------------------------------------------------------
// Component construction and destruction
// ----------------------------------------------------------------------

FlightMain::FlightMain(const char* const compName)
    : FlightMainComponentBase(compName),
      m_min_baro(1e9),
      m_baro_count(0),
      m_drogue_fired(false),
      m_main_fired(false) {}

FlightMain::~FlightMain() {}


void FlightMain::sensorDataIn_handler(FwIndexType portNum, const RitlFsw::SensorData& data) {
    F64 baro = data.get_baro();
    Fw::Logger::log("baro %f\n", baro);

    if (!m_drogue_fired) {

        if (baro < m_min_baro) {
            m_min_baro   = baro;
            m_baro_count = 0;
        } else if (baro > m_min_baro + APOGEE_BARO_DELTA) {
            ++m_baro_count;
        }

        if (m_baro_count >= APOGEE_CONFIRM_COUNT) {

            RitlFsw::ActuationCommand cmd(RitlFsw::CommandId::DROGUE_FIRE);
            Fw::Logger::log("DROGUE FIRED --------------\n");

            this->actuationOut_out(0, cmd);
            m_drogue_fired = true;
        }
    }

    if (m_drogue_fired && !m_main_fired) {

        if (baro >= m_min_baro + MAIN_DEPLOY_DELTA_HPA) {
            RitlFsw::ActuationCommand cmd(RitlFsw::CommandId::MAIN_FIRE);
            Fw::Logger::log("MAIN FIRED --------------\n");

            this->actuationOut_out(0, cmd);
            m_main_fired = true;
        }
    }
}

}  // namespace RitlFsw
