// ======================================================================
// \title  BaroSimSensor.cpp
// \author krissal1234
// \brief  cpp file for BaroSimSensor component implementation class
// ======================================================================

#include "RitlFsw/Components/SimSensors/BaroSimSensor/BaroSimSensor.hpp"

namespace RitlFsw {

// ----------------------------------------------------------------------
// Component construction and destruction
// ----------------------------------------------------------------------

BaroSimSensor ::BaroSimSensor(const char* const compName) : BaroSimSensorComponentBase(compName) {}

BaroSimSensor ::~BaroSimSensor() {}

// ----------------------------------------------------------------------
// Handler implementations for typed input ports
// ----------------------------------------------------------------------

void BaroSimSensor ::baroSensorDataIn_handler(FwIndexType portNum, F32 data) {
    this->m_baro = data;
}

F64 BaroSimSensor ::getBaroData_handler(FwIndexType portNum) {
    return this->m_baro;
}

}  // namespace RitlFsw
