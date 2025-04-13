#pragma once

#include <units.h>

namespace emblib {

/**
 * Used to export the `unit_t` class to the emblib namespace
 * @note Using linear scale for units, this can be changed
 * by adding another template parameter here
 */
template <typename units_type, typename scalar_type>
using unit_t = units::unit_t<units_type, scalar_type>;

}

UNIT_ADD_WITH_CUSTOM_TYPE(emblib, seconds, second, s, unsigned int, units::unit<std::ratio<1>, units::category::time_unit>);
UNIT_ADD_WITH_CUSTOM_TYPE(emblib, milliseconds, millisecond, ms, unsigned int, units::unit<std::ratio<1, 1000>, second>);

namespace emblib {

inline constexpr seconds_t SECONDS_MAX = seconds_t(-1);
inline constexpr milliseconds_t MILLISECONDS_MAX = milliseconds_t(-1);

}