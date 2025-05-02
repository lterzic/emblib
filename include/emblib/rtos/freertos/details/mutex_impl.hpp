#pragma once

#include "emblib/rtos/freertos/semaphore.hpp"
#include "emblib/rtos/mutex.hpp"

namespace emblib::rtos {

inline bool mutex::lock(milliseconds_t timeout) noexcept
{
    return freertos::mutex::take(timeout);
}

inline bool mutex::unlock() noexcept
{
    return freertos::mutex::give();
}

}
