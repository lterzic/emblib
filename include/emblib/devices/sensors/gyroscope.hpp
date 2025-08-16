#pragma once

#include "sensor.hpp"
#include "emblib/units/rotation.hpp"

namespace emblib::devices {

/**
 * Gyroscope sensor type
 * Data type is array of 3 rad/s values
 */
using gyroscope = sensor<units::radians_per_second<float>[3]>;

}
