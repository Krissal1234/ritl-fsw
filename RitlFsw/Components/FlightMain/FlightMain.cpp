// ======================================================================
// \title  FlightMain.cpp
// \author krissal1234
// \brief  cpp file for FlightMain component implementation class
// ======================================================================

#include "RitlFsw/Components/FlightMain/FlightMain.hpp"

namespace RitlFsw {

// ----------------------------------------------------------------------
// Component construction and destruction
// ----------------------------------------------------------------------

FlightMain ::FlightMain(const char* const compName) : FlightMainComponentBase(compName) {}

FlightMain ::~FlightMain() {}

// ----------------------------------------------------------------------
// Handler implementations for typed input ports
// ----------------------------------------------------------------------

void FlightMain ::sensorDataIn_handler(FwIndexType portNum, const RitlFsw::SensorData& data) {
    this->sensorDataToControl_out(0, data);
    this->sensorDatatoRecovery_out(0,data);

    // this->baroToRecovery_out(0, data.get_baro());
}

}  // namespace RitlFsw
