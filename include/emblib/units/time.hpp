#pragma once

#include "units.hpp"

namespace emblib::units {

UNIT_DEFINE(seconds, UNIT_RATIO(::units::category::time_unit, 1));
UNIT_DEFINE(hertz, ::units::inverse<details::seconds_unit>);

UNIT_DEFINE(minute, UNIT_RATIO(details::seconds_unit, 60));
UNIT_DEFINE(milliseconds, UNIT_RATIO(details::seconds_unit, 1, 1000));
UNIT_DEFINE(microseconds, UNIT_RATIO(details::seconds_unit, 1, 1000000));

}