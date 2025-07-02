#pragma once

#include "emblib/emblib.hpp"
#include "emblib/units/speed.hpp"
#include "sensornd.hpp"

namespace emblib::driver {

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

#if EMBLIB_UNNEST_NAMESPACES
namespace emblib {
    using namespace driver;
}
#endif