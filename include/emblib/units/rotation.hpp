#pragma once

#include "angle.hpp"
#include "time.hpp"

namespace emblib::units {

UNIT_DEFINE(radians_per_second, ::units::compound_unit<details::radians_unit, details::hertz_unit>);
UNIT_DEFINE(degrees_per_second, ::units::compound_unit<details::degrees_unit, details::hertz_unit>);
UNIT_DEFINE(revolutions_per_minute, ::units::unit<std::ratio<2, 60>, details::radians_per_second_unit, std::ratio<1>>);

}