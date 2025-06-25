#pragma once

#include "units.hpp"

namespace emblib::units {

UNIT_DEFINE(seconds, UNIT_RATIO(::units::category::time_unit, 1));
UNIT_DEFINE(milliseconds, UNIT_RATIO(details::seconds_unit, 1, 1000));
UNIT_DEFINE(microseconds, UNIT_RATIO(details::seconds_unit, 1, 1000000));

UNIT_DEFINE(meters, UNIT_RATIO(::units::category::length_unit, 1));
UNIT_DEFINE(millimeters, UNIT_RATIO(details::meters_unit, 1, 1000));
UNIT_DEFINE(kilometers, UNIT_RATIO(details::meters_unit, 1000));

UNIT_DEFINE(radians, UNIT_RATIO(::units::category::angle_unit, 1));

UNIT_DEFINE(hertz, ::units::inverse<details::seconds_unit>);

UNIT_DEFINE(meters_per_second, ::units::compound_unit<details::meters_unit, details::hertz_unit>);
UNIT_DEFINE(meters_per_second_squared, ::units::compound_unit<details::meters_unit, ::units::squared<details::hertz_unit>>);
UNIT_DEFINE(radians_per_second, ::units::compound_unit<details::radians_unit, details::hertz_unit>);

}