#pragma once

#include <units.h>

namespace emblib {

using second_t = units::unit<std::ratio<1>, units::category::time_unit>;
using minute_t = units::unit<std::ratio<60>, second_t>;
using millisecond_t = units::unit<std::ratio<1, 1000>, second_t>;

using meter_t = units::unit<std::ratio<1>, units::category::length_unit>;
using kilometer_t = units::unit<std::ratio<1000>, meter_t>;
using millimeter_t = units::unit<std::ratio<1, 1000>, meter_t>;

using radian_t = units::unit<std::ratio<1>, units::category::angle_unit>;
using degree_t = units::unit<std::ratio<1, 180>, radian_t, std::ratio<1>>;
using rotation_t = units::unit<std::ratio<360>, degree_t>;

using meter_per_second_t = units::compound_unit<meter_t, units::inverse<second_t>>;
using meter_per_second_squared_t = units::compound_unit<meter_per_second_t, units::inverse<second_t>>;
using standard_gravity_t = units::unit<std::ratio<980665, 100000>, meter_per_second_squared_t>;

using radian_per_second_t = units::compound_unit<radian_t, units::inverse<second_t>>;
using degree_per_second_t = units::compound_unit<degree_t, units::inverse<second_t>>;
using rotation_per_minute_t = units::compound_unit<rotation_t, units::inverse<minute_t>>;

}