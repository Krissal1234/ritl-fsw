// ======================================================================
// \title  RecoveryController.cpp
// \author krissal1234
// \brief  cpp file for RecoveryController component implementation class
// ======================================================================

#include "RitlFsw/Components/Controllers/RecoveryController/RecoveryController.hpp"
#include "Fw/Logger/Logger.hpp"

namespace RitlFsw {

// ----------------------------------------------------------------------
// Component construction and destruction
// ----------------------------------------------------------------------

RecoveryController ::RecoveryController(const char* const compName) : RecoveryControllerComponentBase(compName),
        m_min_baro(1e9),
        m_baro_count(0),
        m_drogue_fired(false),
        m_main_fired(false) {}

RecoveryController ::~RecoveryController() {}

// ----------------------------------------------------------------------
// Handler implementations for typed input ports
// ----------------------------------------------------------------------

void RecoveryController ::baroDataIn_handler(FwIndexType portNum, F64 data) {
    F32 baro = data;
    if (!m_drogue_fired) {

        if (baro < m_min_baro) {
            m_min_baro   = baro;
            m_baro_count = 0;
        } else if (baro > m_min_baro + APOGEE_BARO_DELTA) {
            ++m_baro_count;
        }

        if (m_baro_count >= APOGEE_CONFIRM_COUNT) {

            RitlFsw::ActuationCommand cmd(RitlFsw::CommandId::DROGUE_FIRE, 0.0);
            Fw::Logger::log("DROGUE FIRED --------------\n");

            this->actuationCommandOut_out(0, cmd);
            m_drogue_fired = true;
        }
    }

    if (m_drogue_fired && !m_main_fired) {

        if (baro >= m_min_baro + MAIN_DEPLOY_DELTA_HPA) {
            RitlFsw::ActuationCommand cmd(RitlFsw::CommandId::MAIN_FIRE,0.0);
            Fw::Logger::log("MAIN FIRED --------------\n");

            this->actuationCommandOut_out(0, cmd);
            m_main_fired = true;
        }
    }
}

}  // namespace RitlFsw
