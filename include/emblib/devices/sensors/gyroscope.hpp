#pragma once

#include "sensor.hpp"
#include "emblib/units/rotation.hpp"
#include <etl/array.h>

namespace emblib::devices {

/**
 * SI rotation units - radians per second
 */
using gyroscope_units = units::radians_per_second<float>;

/**
 * Gyroscope sensor type
 */
using gyroscope = sensor<etl::array<gyroscope_units, 3>>;

}
