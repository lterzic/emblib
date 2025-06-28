#pragma once

#include "units.hpp"

namespace emblib::units {

UNIT_DEFINE(meters, UNIT_RATIO(::units::category::length_unit, 1));
UNIT_DEFINE(millimeters, UNIT_RATIO(details::meters_unit, 1, 1000));
UNIT_DEFINE(kilometers, UNIT_RATIO(details::meters_unit, 1000));

}