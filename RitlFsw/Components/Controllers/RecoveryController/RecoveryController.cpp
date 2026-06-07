#include "RitlFsw/Components/Controllers/RecoveryController/RecoveryController.hpp"
#include "Fw/Logger/Logger.hpp"
#include <cmath>

namespace RitlFsw {

RecoveryController::RecoveryController(const char* const compName)
    : RecoveryControllerComponentBase(compName),
      m_min_baro(1e9),
      m_baro_count(0),
      m_drogue_fired(false),
      m_main_fired(false),
      m_prev_baro(1e9),
      m_baro_frozen_count(0),
      m_baro_frozen(false),
      m_last_t(0.0),
      m_launched(false) {}

RecoveryController::~RecoveryController() {}

void RecoveryController::fireDrogue() {
    RitlFsw::ActuationCommand cmd;
    cmd.set_cmdId(RitlFsw::CommandId::DROGUE_FIRE);
    cmd.set_deployment_level(0.0f);
    this->actuationCommandOut_out(0, cmd);
}

void RecoveryController::fireMain() {
    RitlFsw::ActuationCommand cmd;
    cmd.set_cmdId(RitlFsw::CommandId::MAIN_FIRE);
    cmd.set_deployment_level(0.0f);
    this->actuationCommandOut_out(0, cmd);
}

void RecoveryController::sensorDataIn_handler(FwIndexType portNum, const RitlFsw::SensorData& data) {
    F64 baro = data.get_baro() / 100.0;
    F64 accel_z = data.get_accel().get_z();
    F64 t = data.get_t();

    // --- Launch detection ---
    // IMU reads 0 at rest, so any significant accel means we launched
    if (!m_launched && accel_z > LAUNCH_ACCEL_THRESH) {
        m_launched = true;
        Fw::Logger::log("LAUNCH detected\n");
    }

    // --- Baro free// --- Baro freeze detection --- only after launch
    if (m_launched && m_prev_baro != 1e9) {
        if (std::abs(baro - m_prev_baro) < BARO_FREEZE_DELTA) {
            m_baro_frozen_count++;
            if (m_baro_frozen_count >= BARO_FREEZE_COUNT && !m_baro_frozen) {
                m_baro_frozen = true;
                Fw::Logger::log("BARO FROZEN - switching to IMU fallback\n");
            }
        } else {
            m_baro_frozen_count = 0;
        }
    }
    m_prev_baro = baro;

    // --- dt ---
    F64 dt = t - m_last_t;
    m_last_t = t;

    if (m_drogue_fired) {
        if (!m_main_fired) {
            // Normal baro-based main deploy
            if (!m_baro_frozen && baro >= m_min_baro + MAIN_DEPLOY_DELTA_HPA) {
                this->fireMain();
                Fw::Logger::log("MAIN FIRED (baro) --------------\n");
                m_main_fired = true;
            }
            // Fallback: time-based if baro is frozen
            else if (m_baro_frozen && (t - m_drogue_time) >= MAIN_DEPLOY_TIMEOUT) {
                this->fireMain();
                Fw::Logger::log("MAIN FIRED (timeout fallback) --------------\n");
                m_main_fired = true;
            }
        }
        return;
    }

    if (!m_baro_frozen) {
        // --- Primary: baro apogee detection ---
        if (baro < m_min_baro) {
            m_min_baro   = baro;
            m_baro_count = 0;
        } else if (baro > m_min_baro + APOGEE_BARO_DELTA) {
            ++m_baro_count;
        }
        if (m_baro_count >= APOGEE_CONFIRM_COUNT) {
            this->fireDrogue();
            Fw::Logger::log("DROGUE FIRED (baro) --------------\n");
            m_drogue_time = t;
            m_drogue_fired = true;
        }
    } else {
        // --- Fallback: free-fall detection via IMU ---
        // Since IMU reads 0 at rest AND 0 in free fall,
        // we gate on m_launched to distinguish the two.
        // At apogee the rocket enters free fall: accel_z -> 0
        if (m_launched && std::abs(accel_z) < ACCEL_FREEFALL_THRESH) {
            this->fireDrogue();
            m_drogue_time = t;
            Fw::Logger::log("DROGUE FIRED (IMU fallback) --------------\n");
            m_drogue_fired = true;
        }
    }
}

void RecoveryController::baroDataIn_handler(FwIndexType portNum, F64 data) {
    F32 baro = data / 100;
    if (!m_drogue_fired) {
        if (baro < m_min_baro) {
            m_min_baro   = baro;
            m_baro_count = 0;
        } else if (baro > m_min_baro + APOGEE_BARO_DELTA) {
            ++m_baro_count;
        }
        if (m_baro_count >= APOGEE_CONFIRM_COUNT) {
            this->fireDrogue();
            Fw::Logger::log("DROGUE FIRED --------------\n");
            m_drogue_fired = true;
        }
    }
    if (m_drogue_fired && !m_main_fired) {
        if (baro >= m_min_baro + MAIN_DEPLOY_DELTA_HPA) {
            this->fireMain();
            Fw::Logger::log("MAIN FIRED --------------\n");
            m_main_fired = true;
        }
    }
}

}  // namespace RitlFsw