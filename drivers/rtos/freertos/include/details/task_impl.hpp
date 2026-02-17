#pragma once

#include "emblib/rtos/freertos/task.hpp"
#include "emblib/rtos/task.hpp"

namespace emblib::rtos {

template <size_t STACK_SIZE_BYTES>
inline task::task(const char* name, size_t priority, task_stack<STACK_SIZE_BYTES>& stack) :
    details::task_native_t(name, priority, (freertos::task_stack<sizeof(stack) / sizeof(freertos::task_stack<1>)>&)stack)
{}

inline void task::suspend() noexcept
{
    freertos::task::suspend();
}

inline void task::resume() noexcept
{
    freertos::task::resume();
}

inline void task::notify() noexcept
{
    freertos::task::notify_give();
}

inline void task::notify_from_isr() noexcept
{
    freertos::task::notify_give_from_isr();
}

inline void task::sleep(ticks duration) noexcept
{
    freertos::task::sleep(duration);
}

inline void task::sleep_periodic(ticks period) noexcept
{
    freertos::task::sleep_periodic(period);
}

inline bool task::wait_notification(ticks timeout, bool clear_count) noexcept
{
    return freertos::task::notify_take(timeout, clear_count);
}

}