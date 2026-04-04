#pragma once

#include "sensor.hpp"
#include "emblib/units/speed.hpp"
#include <etl/array.h>

namespace emblib::devices {

/**
 * SI acceleration units - meters per second squared
 */
using accelerometer_units = units::standard_gravity<int16_t>;

/**
 * Accelerometer sensor type
 */
using accelerometer = sensor<etl::array<accelerometer_units, 3>>;

}
