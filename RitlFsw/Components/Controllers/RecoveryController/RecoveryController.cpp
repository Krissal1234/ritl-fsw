// ======================================================================
// \title  RecoveryController.cpp
// \author krissal1234
// \brief  cpp file for RecoveryController component implementation class
// ======================================================================

#include "RitlFsw/Components/Controllers/RecoveryController/RecoveryController.hpp"

namespace RitlFsw {

// ----------------------------------------------------------------------
// Component construction and destruction
// ----------------------------------------------------------------------

RecoveryController ::RecoveryController(const char* const compName) : RecoveryControllerComponentBase(compName) {}

RecoveryController ::~RecoveryController() {}

void RecoveryController::fireDrogue() {
    RitlFsw:ActuationCommand cmd;
    cmd.set_cmdId(RitlFsw::CommandId::DROGUE_FIRE);
    cmd.set_deployment_level(0.0f);
    this->actuationCommandOut_out(0, cmd);
}

void RecoveryController::fireMain() {
    RitlFsw:ActuationCommand cmd;
    cmd.set_cmdId(RitlFsw::CommandId::MAIN_FIRE);
    cmd.set_deployment_level(0.0f);
    this->actuationCommandOut_out(0, cmd);
}

// ----------------------------------------------------------------------
// Handler implementations for typed input ports
// ----------------------------------------------------------------------

void RecoveryController ::baroDataIn_handler(FwIndexType portNum, F64 data) {

}

}  // namespace RitlFsw
