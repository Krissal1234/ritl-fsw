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

bool SensorHub ::getSimReady_handler(FwIndexType portNum) {
  return this->m_has_received_data;
}

void SensorHub ::sensorDataIn_handler(FwIndexType portNum, const RitlFsw::SensorData& data) {
  this->m_has_received_data = true;
  this->m_sensorData = data;

  RitlFsw::ImuSimData imu;
  imu.set_accel(data.get_accel());
  imu.set_gyro(data.get_gyro());
  this->imuDataOut_out(0,imu);

  this->baroDataOut_out(0,data.get_baro());
}

}  // namespace RitlFsw
