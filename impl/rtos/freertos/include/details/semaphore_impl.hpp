#pragma once

#include "emblib/rtos/freertos/semaphore.hpp"
#include "emblib/rtos/semaphore.hpp"

namespace emblib::rtos {

inline bool semaphore::wait(milliseconds<size_t> timeout) noexcept
{
    return freertos::semaphore::take(timeout);
}

inline bool semaphore::signal() noexcept
{
    return freertos::semaphore::give();
}

inline bool semaphore::signal_from_isr() noexcept
{
    return freertos::semaphore::give_from_isr();
}

}
