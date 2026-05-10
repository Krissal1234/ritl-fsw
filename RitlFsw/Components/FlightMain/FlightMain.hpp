// ======================================================================
// \title  FlightMain.hpp
// \author krissal1234
// \brief  hpp file for FlightMain component implementation class
// ======================================================================

#ifndef RitlFsw_FlightMain_HPP
#define RitlFsw_FlightMain_HPP

#include "RitlFsw/Components/FlightMain/FlightMainComponentAc.hpp"

namespace RitlFsw {

class FlightMain final : public FlightMainComponentBase {
  public:
    // ----------------------------------------------------------------------
    // Component construction and destruction
    // ----------------------------------------------------------------------

    //! Construct FlightMain object
    FlightMain(const char* const compName  //!< The component name
    );

    //! Destroy FlightMain object
    ~FlightMain();

  private:
    // ----------------------------------------------------------------------
    // Handler implementations for typed input ports
    // ----------------------------------------------------------------------

    //! Handler implementation for sensorDataIn
    //!
    //! OrchReceiver drives the main component
    void sensorDataIn_handler(FwIndexType portNum,  //!< The port number
                              const RitlFsw::SensorData& data) override;
};

}  // namespace RitlFsw

#endif
