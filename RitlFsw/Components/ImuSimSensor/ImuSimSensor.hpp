// ======================================================================
// \title  ImuSimSensor.hpp
// \author krissal1234
// \brief  hpp file for ImuSimSensor component implementation class
// ======================================================================

#ifndef RitlFsw_ImuSimSensor_HPP
#define RitlFsw_ImuSimSensor_HPP

#include "RitlFsw/Components/ImuSimSensor/ImuSimSensorComponentAc.hpp"

namespace RitlFsw {

class ImuSimSensor final : public ImuSimSensorComponentBase {
  public:
    // ----------------------------------------------------------------------
    // Component construction and destruction
    // ----------------------------------------------------------------------

    //! Construct ImuSimSensor object
    ImuSimSensor(const char* const compName  //!< The component name
    );

    //! Destroy ImuSimSensor object
    ~ImuSimSensor();

  private:
    RitlFsw::ImuSimData m_imuData {};
    // ----------------------------------------------------------------------
    // Handler implementations for typed input ports
    // ----------------------------------------------------------------------

    //! Handler implementation for getImuData
    //!
    //! Polling port from FlightMain to receive imu data
    RitlFsw::ImuSimData getImuData_handler(FwIndexType portNum  //!< The port number
                                           ) override;

    //! Handler implementation for imuSensorDataIn
    //!
    //! Incoming data from Receiver
    void imuSensorDataIn_handler(FwIndexType portNum,  //!< The port number
                                 const RitlFsw::ImuSimData& data) override;
};

}  // namespace RitlFsw

#endif
