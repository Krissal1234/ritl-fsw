// ======================================================================
// \title  ImuSimSensor.cpp
// \author krissal1234
// \brief  cpp file for ImuSimSensor component implementation class
// ======================================================================

#include "RitlFsw/Components/SimSensors/ImuSimSensor/ImuSimSensor.hpp"
#include "Fw/Logger/Logger.hpp"

namespace RitlFsw {

// ----------------------------------------------------------------------
// Component construction and destruction
// ----------------------------------------------------------------------

ImuSimSensor ::ImuSimSensor(const char* const compName) : ImuSimSensorComponentBase(compName) {}

ImuSimSensor ::~ImuSimSensor() {}

// ----------------------------------------------------------------------
// Handler implementations for typed input ports
// ----------------------------------------------------------------------

RitlFsw::ImuSimData ImuSimSensor ::getImuData_handler(FwIndexType portNum) {
    return this->m_imuData;
}

void ImuSimSensor ::imuSensorDataIn_handler(FwIndexType portNum, const RitlFsw::ImuSimData& data) {
    this->m_imuData = data;
}

}  // namespace RitlFsw
