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
    RitlFsw::ImuSimData imu = this->getImu_out(0);
    F64 baro = this->getBaro_out(0);

    if (m_state == FlightState::INITIALISING) {
        bool ready = this->getSimReady_out(0);
        if (!ready) return;  // no data yet

        // first real data arrived
        m_state = FlightState::FLIGHT;
        Fw::Logger::log("FlightMain: sim connected\n");
    }
    RitlFsw::SensorData sd;

    sd.set_accel(imu.get_accel());
    sd.set_gyro(imu.get_gyro());
    sd.set_baro(baro);
    sd.set_t(0.0); // we should look into this when we start measuring latencies

    this->baroToRecovery_out(0, baro);
    this->sensorDataToControl_out(0,sd);


}

}  // namespace RitlFsw
