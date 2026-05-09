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
    enum class FlightState { INITIALISING, FLIGHT, LANDED };

    FlightState m_state = FlightState::INITIALISING;






    // ----------------------------------------------------------------------
    // Handler implementations for typed input ports
    // ----------------------------------------------------------------------

    //! Handler implementation for run
    //!
    //! Scheduler that drives the main component
    void run_handler(FwIndexType portNum,  //!< The port number
                     U32 context           //!< The call order
                     ) override;
};

}  // namespace RitlFsw

#endif
