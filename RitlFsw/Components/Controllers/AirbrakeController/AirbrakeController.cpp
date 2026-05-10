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

void AirbrakeController::setAirbrake(F64 level) {
    RitlFsw::ActuationCommand cmd;
    cmd.set_cmdId(RitlFsw::CommandId::AIRBRAKE_SET);
    cmd.set_deployment_level(level);
    this->actuationCommandOut_out(0, cmd);
}

void AirbrakeController::sensorDataIn_handler(
    FwIndexType portNum,
    const RitlFsw::SensorData& data) {

    F64 t = data.get_t();

    // duplicate timestamp guard
    if (t <= m_prev_sim_time && m_prev_sim_time >= 0.0) {
        return;
    }
    m_prev_sim_time = t;

    F64 dt = (m_prev_sim_time_for_dt >= 0.0) ? (t - m_prev_sim_time_for_dt) : (1.0 / CALL_RATE_HZ);
    m_prev_sim_time_for_dt = t;

    F64 pressure = data.get_baro();
    F64 az = data.get_accel().get_z();

    // set ground pressure on first call
    if (m_P0 < 0.0) {
        m_P0 = pressure;
        m_prev_alt = 0.0;
        this->setAirbrake(0.0);
        return;
    }

    // always integrate vz
    m_vz += az * dt;

    // only act after burnout
    if (!m_burned_out) {
        if (az < BOOST_ACCEL_THRESHOLD) {
            m_burned_out = true;
        } else {
            this->setAirbrake(0.0);
            return;
        }
    }

    F64 altitude = 44330.0 * (1.0 - pow(pressure / m_P0, 1.0 / 5.255));

    // descent detection — proxy for parachute deployed
    if (altitude < m_prev_alt) {
        m_descent_count++;
    } else {
        m_descent_count = 0;
    }

    if (m_descent_count >= DESCENT_COUNT_THRESHOLD) {
        m_integral = 0.0;
        m_vz       = 0.0;
        m_prev_alt = altitude;
        this->setAirbrake(0.0);
        return;
    }

    m_prev_alt = altitude;

    F64 predicted_apogee = altitude + (m_vz * m_vz) / (2.0 * g);
    F64 error = predicted_apogee - TARGET_APOGEE;

    m_integral += error * dt;
    if (m_integral >  INTEGRAL_CLAMP) m_integral =  INTEGRAL_CLAMP;
    if (m_integral < -INTEGRAL_CLAMP) m_integral = -INTEGRAL_CLAMP;

    F64 raw = Kp * error + Ki * m_integral;
    if (raw < 0.0) raw = 0.0;
    if (raw > 1.0) raw = 1.0;

    Fw::Logger::log("t=%.2f alt=%.1f pred=%.1f err=%.1f dep=%.3f vz=%.2f\n",
        t, altitude, predicted_apogee, error, raw, m_vz);

    this->setAirbrake(raw);
}

}  // namespace RitlFsw