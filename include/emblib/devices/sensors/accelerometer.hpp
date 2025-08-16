#pragma once

#include "emblib/units/speed.hpp"
#include "sensornd.hpp"

namespace emblib::devices {

/**
 * Acceleration units in meters per second^2
 */
using mpss_t = units::meters_per_second_squared<float>;

/**
 * Accelerometer interface
 */
class accelerometer : public sensornd<mpss_t> {

};

}
