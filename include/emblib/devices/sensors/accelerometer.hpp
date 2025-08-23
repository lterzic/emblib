#pragma once

#include "sensor.hpp"
#include "emblib/units/speed.hpp"
#include <etl/array.h>

namespace emblib::devices {

/**
 * SI acceleration units - meters per second squared
 */
using accelerometer_units = units::meters_per_second_squared<float>;

/**
 * Accelerometer sensor type
 */
using accelerometer = sensor<etl::array<accelerometer_units, 3>>;

}
