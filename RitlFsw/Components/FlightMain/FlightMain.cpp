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
    // F64 baro = this->getBaro_out(0);
    if (m_state == FlightState::INITIALISING) {
        bool ready = this->getSimReady_out(0);
        if (!ready) return;
        m_state = FlightState::FLIGHT;
        Fw::Logger::log("FlightMain: sim connected\n");
    }

    // only reached once in FLIGHT state
    RitlFsw::SensorData sd = this->getSensorData_out(0);
    // Fw::Logger::log("Received sensor data in main: t=%.4f\n", sd.get_t());
    this->sensorDataToControl_out(0, sd);
    this->baroToRecovery_out(0, sd.get_baro());
    // RitlFsw::SensorData sd;

    // sd.set_accel(imu.get_accel());
    // sd.set_gyro(imu.get_gyro());
    // sd.set_baro(baro);
    // sd.set_t(0.0); // we should look into this when we start measuring latencies



}

}  // namespace RitlFsw
