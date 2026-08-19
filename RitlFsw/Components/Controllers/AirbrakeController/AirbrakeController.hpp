// ======================================================================
// \title  AirbrakeController.hpp
// \author krissal1234
// \brief  hpp file for AirbrakeController component implementation class
// ======================================================================

#ifndef RitlFsw_AirbrakeController_HPP
#define RitlFsw_AirbrakeController_HPP

#include "RitlFsw/Components/Controllers/AirbrakeController/AirbrakeControllerComponentAc.hpp"

namespace RitlFsw {

class AirbrakeController final : public AirbrakeControllerComponentBase {
  public:
    // ----------------------------------------------------------------------
    // Component construction and destruction
    // ----------------------------------------------------------------------

    //! Construct AirbrakeController object
    AirbrakeController(const char* const compName  //!< The component name
    );

    //! Destroy AirbrakeController object
    ~AirbrakeController();

  private:
    void setAirbrake(F64 level);


    // ----------------------------------------------------------------------
    // Handler implementations for typed input ports
    // ----------------------------------------------------------------------

    //! Handler implementation for sensorDataIn
    void sensorDataIn_handler(FwIndexType portNum,  //!< The port number
                              const RitlFsw::SensorData& data) override;
};

}  // namespace RitlFsw

#endif
