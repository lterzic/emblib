#pragma once

#include "details/chrono_native.hpp"
#include <chrono>

namespace emblib::rtos {

/**
 * Ticks are the basic unit of RTOS time measurement.
 */
using ticks = details::ticks_native;

/**
 * Steady clock based on the number of ticks since system start.
 */
struct tick_clock {
    using duration = ticks;
    using time_point = std::chrono::time_point<tick_clock, duration>;

    static constexpr bool is_steady = true;

    static time_point now() noexcept;
};

}

#include "details/chrono_impl.hpp"