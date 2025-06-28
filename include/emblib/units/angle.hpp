#pragma once

#include "units.hpp"

namespace emblib::units {

UNIT_DEFINE(radians, UNIT_RATIO(::units::category::angle_unit, 1));
UNIT_DEFINE(degrees, ::units::unit<std::ratio<1, 180>, details::radians_unit, std::ratio<1>>);

}