#pragma once

#include "emblib/emblib.hpp"
#include "sensor3d.hpp"
#include "emblib/utils/units.hpp"

namespace emblib::driver {

/**
 * Angular velocity units in radians per second
 */
using rps_t = units::unit_t<radian_per_second_t, float>;

/**
 * Angular velocity units in degrees per second
 */
using dps_t = units::unit_t<degree_per_second_t, float>;

/**
 * Gyroscope interface
 * @note Data type of an gyroscope must have
 * dimensions of angle units / time
 */
template <
    typename data_type = dps_t,
    typename = std::enable_if_t<
        std::is_arithmetic_v<data_type> ||
        are_same_dim_units<dps_t::unit_type, typename data_type::unit_type>
    >
>
class gyroscope : public sensor3d<data_type> {

};

}

#if EMBLIB_UNNEST_NAMESPACES
namespace emblib {
    using namespace driver;
}
#endif