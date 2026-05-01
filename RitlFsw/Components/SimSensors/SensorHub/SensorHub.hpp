// ======================================================================
// \title  SensorHub.hpp
// \author krissal1234
// \brief  hpp file for SensorHub component implementation class
// ======================================================================

#ifndef RitlFsw_SensorHub_HPP
#define RitlFsw_SensorHub_HPP

#include "RitlFsw/Components/SimSensors/SensorHub/SensorHubComponentAc.hpp"

namespace RitlFsw {

class SensorHub final : public SensorHubComponentBase {
  public:
    // ----------------------------------------------------------------------
    // Component construction and destruction
    // ----------------------------------------------------------------------

    //! Construct SensorHub object
    SensorHub(const char* const compName  //!< The component name
    );

    //! Destroy SensorHub object
    ~SensorHub();

    private:
      RitlFsw::SensorData m_sensorData {
          0.0,        // t — time, 0 on startup
          {0.0, 0.0, 0.0},  // accel
          1e9,   // baro — starts very high
          {0.0, 0.0, 0.0}   // gyro
      };
    // ----------------------------------------------------------------------
    // Handler implementations for typed input ports
    // ----------------------------------------------------------------------

    //! Handler implementation for getSensorData
    //!
    //! Future proof hook in case testing requires bypassing SimSensors
    RitlFsw::SensorData getSensorData_handler(FwIndexType portNum  //!< The port number
                                              ) override;

    //! Handler implementation for sensorDataIn
    //!
    //! Receive deserialized sensor data from OrchReceiver
    void sensorDataIn_handler(FwIndexType portNum,  //!< The port number
                              const RitlFsw::SensorData& data) override;
};

}  // namespace RitlFsw

#endif
