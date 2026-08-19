// ======================================================================
// \title  AirbrakeController.cpp
// \author krissal1234
// \brief  cpp file for AirbrakeController component implementation class
// ======================================================================

#include "RitlFsw/Components/Controllers/AirbrakeController/AirbrakeController.hpp"
#include "Fw/Logger/Logger.hpp"
#include <algorithm>
#include <cmath>

namespace RitlFsw {
AirbrakeController::AirbrakeController(const char* const compName)
    : AirbrakeControllerComponentBase(compName) {}

AirbrakeController::~AirbrakeController() {}

void AirbrakeController::setAirbrake(F64 level) {
    RitlFsw::ActuationCommand cmd;
    cmd.set_cmdId(RitlFsw::CommandId::AIRBRAKE_SET);
    cmd.set_deployment_level(level);
    this->actuationCommandOut_out(0, cmd);
}

void AirbrakeController::sensorDataIn_handler(FwIndexType portNum, const RitlFsw::SensorData& data) {

}
}  // namespace RitlFsw
