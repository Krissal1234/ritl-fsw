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

FlightMain ::FlightMain(const char* const compName) : FlightMainComponentBase(compName),
        m_min_baro(1e9),
        m_baro_count(0),
        m_drogue_fired(false),
        m_main_fired(false) {}

FlightMain ::~FlightMain() {};

// ----------------------------------------------------------------------
// Handler implementations for typed input ports
// ----------------------------------------------------------------------

void FlightMain::fireDrogue() {
    RitlFsw:ActuationCommand cmd;
    cmd.set_cmdId(RitlFsw::CommandId::DROGUE_FIRE);
    cmd.set_deployment_level(0.0f);
    this->actuationCommandOut_out(0, cmd);
}

void FlightMain::fireMain() {
    RitlFsw:ActuationCommand cmd;
    cmd.set_cmdId(RitlFsw::CommandId::MAIN_FIRE);
    cmd.set_deployment_level(0.0f);
    this->actuationCommandOut_out(0, cmd);
}

void FlightMain ::run_handler(FwIndexType portNum, U32 context) {
    if (m_state == FlightState::INITIALISING) {
        bool ready = this->getSimReady_out(0);
        if (!ready) return;
        m_state = FlightState::FLIGHT;
        Fw::Logger::log("FlightMain: sim connected\n");
    }

    RitlFsw::ImuSimData imu = this->getImu_out(0);
    F64 baro = this->getBaro_out(0) / 100;

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
    // RitlFsw::SensorData sd;

    // sd.set_accel(imu.get_accel());
    // sd.set_gyro(imu.get_gyro());
    // sd.set_baro(baro);
    // sd.set_t(0.0); // we should look into this when we start measuring latencies

    // only reached once in FLIGHT state
    RitlFsw::SensorData sd = this->getSensorData_out(0);
    // Fw::Logger::log("Received sensor data in main: t=%.4f\n", sd.get_t());

    this->sensorDataToControl_out(0, sd);


}

}  // namespace RitlFsw
