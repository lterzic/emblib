#pragma once

#include "emblib/rtos/freertos/chrono.hpp"
#include "emblib/rtos/chrono.hpp"

namespace emblib::rtos {

inline tick_clock::time_point tick_clock::now() noexcept
{
    return time_point(freertos::get_ticks());
}

}
