#pragma once

#include "length.hpp"
#include "time.hpp"

namespace emblib::units {

UNIT_DEFINE(meters_per_second, ::units::compound_unit<details::meters_unit, details::hertz_unit>);
UNIT_DEFINE(meters_per_second_squared, ::units::compound_unit<details::meters_unit, ::units::squared<details::hertz_unit>>);
UNIT_DEFINE(standard_gravity, UNIT_RATIO(details::meters_per_second_squared_unit, 980665, 100000));

}