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

FlightMain ::FlightMain(const char* const compName) : FlightMainComponentBase(compName) {}

FlightMain ::~FlightMain() {}

// ----------------------------------------------------------------------
// Handler implementations for typed input ports
// ----------------------------------------------------------------------

void FlightMain ::run_handler(FwIndexType portNum, U32 context) {
    // RitlFsw::ImuSimData imu = this->getImu_out(0);

    if (m_state == FlightState::INITIALISING) {
        bool ready = this->getSimReady_out(0);
        if (!ready) return;
        m_state = FlightState::FLIGHT;
        Fw::Logger::log("FlightMain: sim connected\n");
    }

    RitlFsw::SensorData sd = this->getSensorData_out(0);
    this->sensorDataToControl_out(0, sd);
    this->baroToRecovery_out(0, sd.get_baro());



}

}  // namespace RitlFsw
