// ======================================================================
// \title  AirbrakeController.cpp
// \author krissal1234
// \brief  cpp file for AirbrakeController component implementation class
// ======================================================================

#include "RitlFsw/Components/Controllers/AirbrakeController/AirbrakeController.hpp"
#include "Fw/Logger/Logger.hpp"
#include <cmath>

namespace RitlFsw {

AirbrakeController::AirbrakeController(const char* const compName)
    : AirbrakeControllerComponentBase(compName) {}

AirbrakeController::~AirbrakeController() {}

void AirbrakeController::sensorDataIn_handler(
    FwIndexType portNum,
    const RitlFsw::SensorData& data) {

    const F64 dt = 1.0 / CALL_RATE_HZ;

    F64 pressure = data.get_baro();
    F64 ax = data.get_accel().get_x();
    F64 ay = data.get_accel().get_y();
    F64 az = data.get_accel().get_z();

    if (m_P0 < 0.0) {
        m_P0 = pressure;
        m_prev_alt = 0.0;
        return;  // skip first call, just initialise
    }

    F64 altitude = 44330.0 * (1.0 - pow(pressure / m_P0, 1.0 / 5.255));

    // still burning
    if (!m_burned_out) {
        if (az > BOOST_ACCEL_THRESHOLD) {
            // still burning
            m_vz       = 0.0;
            m_prev_alt = altitude;
            if (!m_burnout_retract_sent) { //only send once
                this->sendActuation(0.0);
                m_burnout_retract_sent = true;
            }
            return;
        } else {
            m_burned_out = true;
        }
    }

    m_vz += az * dt;

    if (altitude < m_prev_alt) {
        m_descent_count++;
    } else {
        m_descent_count = 0;
    }

    if (m_descent_count >= DESCENT_COUNT_THRESHOLD) {
        if (!m_apogee_retract_sent) {
            m_apogee_retract_sent = true;
            this->sendActuation(0.0);
        }

        // past apogee - retract
        m_integral     = 0.0;
        m_burned_out   = false;
        m_descent_count = 0;
        m_vz           = 0.0;
        m_prev_alt = altitude;
        return;
    }

    m_prev_alt = altitude;

    F64 predicted_apogee = altitude + (m_vz * m_vz) / (2.0 * g);

    F64 error = predicted_apogee - TARGET_APOGEE;
    m_integral  += error * dt;

    if (m_integral >  INTEGRAL_CLAMP) m_integral =  INTEGRAL_CLAMP;
    if (m_integral < -INTEGRAL_CLAMP) m_integral = -INTEGRAL_CLAMP;

    F64 raw = Kp * error + Ki * m_integral;

    // clamp to [0, 1]
    if (raw < 0.0) raw = 0.0;
    if (raw > 1.0) raw = 1.0;

    Fw::Logger::log("alt=%.1f pred=%.1f err=%.1f dep=%.3f\n",
                    altitude, predicted_apogee, error, raw);

    this->sendActuation(static_cast<F32>(raw));
}

void AirbrakeController::sendActuation(F32 level) {
    RitlFsw::ActuationCommand cmd(RitlFsw::CommandId::AIRBRAKE_SET, level);
    this->actuationCommandOut_out(0, cmd);
}

}  // namespace RitlFsw