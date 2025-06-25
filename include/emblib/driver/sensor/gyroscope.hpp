#pragma once

#include "emblib/emblib.hpp"
#include "sensor3d.hpp"
#include "emblib/units/si.hpp"

namespace emblib::driver {

/**
 * Angular velocity units in radians per second
 */
using rps_t = units::radians_per_second<float>;

/**
 * Gyroscope interface
 */
class gyroscope : public sensor3d<rps_t> {

};

}

#if EMBLIB_UNNEST_NAMESPACES
namespace emblib {
    using namespace driver;
}
#endif