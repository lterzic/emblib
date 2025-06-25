#pragma once

#include "emblib/units/si.hpp"

namespace emblib {

using seconds_t = units::seconds<size_t>;
using milliseconds_t = units::milliseconds<size_t>;

inline constexpr seconds_t SECONDS_MAX = seconds_t(-1);
inline constexpr milliseconds_t MILLISECONDS_MAX = milliseconds_t(-1);

}