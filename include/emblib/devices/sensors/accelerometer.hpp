#pragma once

#include "sensor.hpp"
#include "emblib/units/speed.hpp"

namespace emblib::devices {

/**
 * Accelerometer sensor type
 * Data type is array of 3 m/s^2 values
 */
using accelerometer = sensor<units::meters_per_second_squared<float>[3]>;

}
