#pragma once

#include "emblib/rtos/freertos/chrono.hpp"
#include "emblib/rtos/chrono.hpp"

namespace emblib::rtos {

inline ticks tick_clock::get_ticks() noexcept
{
    return freertos::get_ticks();
}

}
