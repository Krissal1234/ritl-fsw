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

void RecoveryController::fireDrogue() {
    RitlFsw:ActuationCommand cmd;
    cmd.set_cmdId(RitlFsw::CommandId::DROGUE_FIRE);
    cmd.set_deployment_level(0.0f);
    this->actuationCommandOut_out(0, cmd);
}

void RecoveryController::fireMain() {
    RitlFsw:ActuationCommand cmd;
    cmd.set_cmdId(RitlFsw::CommandId::MAIN_FIRE);
    cmd.set_deployment_level(0.0f);
    this->actuationCommandOut_out(0, cmd);
}

// ----------------------------------------------------------------------
// Handler implementations for typed input ports
// ----------------------------------------------------------------------

void RecoveryController ::baroDataIn_handler(FwIndexType portNum, F64 data) {
    F32 baro = data / 100;
    if (!m_drogue_fired) {

        if (baro < m_min_baro) {
            m_min_baro   = baro;
            m_baro_count = 0;
        } else if (baro > m_min_baro + APOGEE_BARO_DELTA) {
            ++m_baro_count;
        }

        if (m_baro_count >= APOGEE_CONFIRM_COUNT) {
            this->fireDrogue();
            Fw::Logger::log("DROGUE FIRED --------------\n");
            m_drogue_fired = true;
        }
    }

    if (m_drogue_fired && !m_main_fired) {

        if (baro >= m_min_baro + MAIN_DEPLOY_DELTA_HPA) {
            this->fireMain();
            Fw::Logger::log("MAIN FIRED --------------\n");
            m_main_fired = true;
        }
    }
}

}  // namespace RitlFsw
