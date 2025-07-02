#pragma once

#include "emblib/emblib.hpp"
#include "emblib/units/rotation.hpp"
#include "sensornd.hpp"

namespace emblib::driver {

/**
 * Angular velocity units in radians per second
 */
using rps_t = units::radians_per_second<float>;

/**
 * Gyroscope interface
 */
class gyroscope : public sensornd<rps_t> {

};

}

#if EMBLIB_UNNEST_NAMESPACES
namespace emblib {
    using namespace driver;
}
#endif