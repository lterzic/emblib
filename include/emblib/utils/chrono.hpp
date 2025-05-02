#pragma once

#include "units.hpp"

namespace emblib {

using seconds_t = units::unit_t<second_t, unsigned int>;
using milliseconds_t = units::unit_t<millisecond_t, unsigned int>;

inline constexpr seconds_t SECONDS_MAX = seconds_t(-1);
inline constexpr milliseconds_t MILLISECONDS_MAX = milliseconds_t(-1);

}