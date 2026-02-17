#pragma once

#include <units.h>

#define UNIT_DEFINE(unit_name, ...) \
    namespace details { \
        using unit_name ## _unit = __VA_ARGS__;} \
    template <typename underlying_type> \
    using unit_name = ::units::unit_t< \
        details::unit_name ## _unit, \
        underlying_type \
    >

#define UNIT_RATIO(base_unit, ...) \
    ::units::unit<std::ratio<__VA_ARGS__>, base_unit>
