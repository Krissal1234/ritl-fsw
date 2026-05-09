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

FlightMain ::FlightMain(const char* const compName) : FlightMainComponentBase(compName) {};
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

    // In place to begin flight computer on first sensor data transmission from simulation
    if (m_state == FlightState::INITIALISING) {
        bool ready = this->getSimReady_out(0);
        if (!ready) return;
        m_state = FlightState::FLIGHT;
        Fw::Logger::log("FlightMain: sim connected\n");
    }

    RitlFsw::ImuSimData imu = this->getImu_out(0);
    F64 baro = this->getBaro_out(0);
    Fw::Logger::log("baro %f\n", baro);

    // only reached once in FLIGHT state

    // Use this for Bonus Project - AirbrakeController requires time t from SensorData
    // So we bypass SimSensors.
    // RitlFsw::SensorData sd = this->getSensorData_out(0);
    // this->sensorDataToControl_out(0, sd);


}

}  // namespace RitlFsw
