// ======================================================================
// \title  SensorHub.cpp
// \author krissal1234
// \brief  cpp file for SensorHub component implementation class
// ======================================================================

#include "RitlFsw/Components/SimSensors/SensorHub/SensorHub.hpp"

namespace RitlFsw {

// ----------------------------------------------------------------------
// Component construction and destruction
// ----------------------------------------------------------------------

SensorHub ::SensorHub(const char* const compName) : SensorHubComponentBase(compName) {}

SensorHub ::~SensorHub() {}

// ----------------------------------------------------------------------
// Handler implementations for typed input ports
// ----------------------------------------------------------------------

RitlFsw::SensorData SensorHub ::getSensorData_handler(FwIndexType portNum) {
  return this->m_sensorData;
}

void SensorHub ::sensorDataIn_handler(FwIndexType portNum, const RitlFsw::SensorData& data) {
  this->m_sensorData = data;
}

}  // namespace RitlFsw
