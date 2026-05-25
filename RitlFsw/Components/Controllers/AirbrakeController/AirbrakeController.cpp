#include "RitlFsw/Components/Controllers/AirbrakeController/AirbrakeController.hpp"
#include "Fw/Logger/Logger.hpp"
#include <cmath>
#include <algorithm>

namespace RitlFsw {

constexpr F64 AirbrakeController::INTEGRAL_CLAMP;

AirbrakeController::AirbrakeController(const char* const compName)
    : AirbrakeControllerComponentBase(compName),
      m_prev_sim_time(-1.0),
      m_P0(-1.0),
      m_vz(0.0),
      m_integral(0.0),
      m_burned_out(false),
      m_boosting(false),
      m_prev_alt(0.0),
      m_descent_count(0) {}

AirbrakeController::~AirbrakeController() {}

void AirbrakeController::setAirbrake(F64 level) {
    RitlFsw::ActuationCommand cmd;
    cmd.set_cmdId(RitlFsw::CommandId::AIRBRAKE_SET);
    cmd.set_deployment_level(level);
    this->actuationCommandOut_out(0, cmd);
}

void AirbrakeController::sensorDataIn_handler(FwIndexType portNum, const RitlFsw::SensorData& data) {
    F64 t = data.get_t();

    if (m_prev_sim_time < 0.0) {
        m_prev_sim_time = t;
        m_P0 = data.get_baro();
        this->setAirbrake(0.0);
        return;
    }
    if (t <= m_prev_sim_time) {
        return;
    }

    F64 dt = t - m_prev_sim_time;
    m_prev_sim_time = t;

    F64 pressure = data.get_baro();
    F64 az = data.get_accel().get_z();

    m_vz += az * dt;

    if (!m_burned_out) {
        if (az > BOOST_ACCEL_THRESHOLD) {
            m_boosting = true;
        }
        if (m_boosting && az < BOOST_ACCEL_THRESHOLD) {
            m_burned_out = true;
            Fw::Logger::log("BURNOUT detected at t=%.2f\n", t);
        } else {
            this->setAirbrake(0.0);
            return;
        }
    }

    F64 altitude = 44330.0 * (1.0 - pow(pressure / m_P0, 1.0 / 5.255));

    if (altitude < m_prev_alt) {
        m_descent_count++;
    } else {
        m_descent_count = 0;
    }
    m_prev_alt = altitude;

    if (m_descent_count >= DESCENT_COUNT_THRESHOLD) {
        this->setAirbrake(0.0);
        return;
    }

    F64 predicted_apogee = altitude + (m_vz * m_vz) / (2.0 * 9.81);

    F64 error = predicted_apogee - TARGET_APOGEE;

    m_integral += error * dt;
    m_integral = std::max(-INTEGRAL_CLAMP, std::min(INTEGRAL_CLAMP, m_integral));

    F64 raw = (Kp * error) + (Ki * m_integral);
    F64 final_dep = std::max(0.0, std::min(1.0, raw));

    this->setAirbrake(final_dep);

    Fw::Logger::log("t=%.2f alt=%.1f pred=%.1f err=%.1f dep=%.3f vz=%.2f\n",
                    t, altitude, predicted_apogee, error, final_dep, m_vz);
}

}