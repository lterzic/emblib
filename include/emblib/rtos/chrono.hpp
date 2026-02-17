#pragma once

#include "details/chrono_native.hpp"

namespace emblib::rtos {

/**
 * Ticks are the basic unit of RTOS time measurement.
 */
using ticks = details::ticks_native;

/**
 * Steady clock based on the number of ticks since system start.
 */
struct tick_clock {
    static ticks get_ticks() noexcept;
};

/**
 * Maximum number of ticks is used to specify an infinite timeout.
 */
constexpr inline ticks MAX_TICKS {(size_t)-1};

}

#include "details/chrono_impl.hpp"