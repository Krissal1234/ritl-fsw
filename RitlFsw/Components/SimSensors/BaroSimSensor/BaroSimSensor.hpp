// ======================================================================
// \title  BaroSimSensor.hpp
// \author krissal1234
// \brief  hpp file for BaroSimSensor component implementation class
// ======================================================================

#ifndef RitlFsw_BaroSimSensor_HPP
#define RitlFsw_BaroSimSensor_HPP

#include "RitlFsw/Components/SimSensors/BaroSimSensor/BaroSimSensorComponentAc.hpp"

namespace RitlFsw {

class BaroSimSensor final : public BaroSimSensorComponentBase {
  public:
    // ----------------------------------------------------------------------
    // Component construction and destruction
    // ----------------------------------------------------------------------

    //! Construct BaroSimSensor object
    BaroSimSensor(const char* const compName  //!< The component name
    );

    //! Destroy BaroSimSensor object
    ~BaroSimSensor();

  private:
    F64 m_baro {1e9};
    // ----------------------------------------------------------------------
    // Handler implementations for typed input ports
    // ----------------------------------------------------------------------

    //! Handler implementation for baroSensorDataIn
    void baroSensorDataIn_handler(FwIndexType portNum,  //!< The port number
                                  F64 data) override;

    //! Handler implementation for getBaroData
    F64 getBaroData_handler(FwIndexType portNum  //!< The port number
                            ) override;
};

}  // namespace RitlFsw

#endif
