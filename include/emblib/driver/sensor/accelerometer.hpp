#pragma once

#include "emblib/emblib.hpp"
#include "sensor3d.hpp"
#include "emblib/utils/units.hpp"

namespace emblib::driver {

/**
 * Acceleration units in meters per second^2
 */
using mpss_t = units::unit_t<meter_per_second_squared_t, float>;

/**
 * Acceleration units in standard gravity
 */
using stdg_t = units::unit_t<standard_gravity_t, float>;

/**
 * Accelerometer interface
 * @note Data type of an accelerometer must have
 * dimensions of length / time^2
 */
template <
    typename data_type = stdg_t,
    typename = std::enable_if_t<
        std::is_arithmetic_v<data_type> ||
        are_same_dim_units<mpss_t::unit_type, typename data_type::unit_type>
    >
>
class accelerometer : public sensor3d<data_type> {

};

}

#if EMBLIB_UNNEST_NAMESPACES
namespace emblib {
    using namespace driver;
}
#endif