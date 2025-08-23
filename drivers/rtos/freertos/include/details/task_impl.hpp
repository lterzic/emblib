#pragma once

#include "emblib/rtos/freertos/task.hpp"
#include "emblib/rtos/task.hpp"

namespace emblib::rtos {

template <size_t STACK_SIZE_BYTES>
inline task::task(const char* name, size_t priority, task_stack<STACK_SIZE_BYTES>& stack) :
    details::task_native_t(name, priority, (freertos::task_stack<sizeof(stack) / sizeof(freertos::task_stack<1>)>&)stack)
{}

inline void task::sleep(units::milliseconds<size_t> duration)
{
    /// @todo Implement in FreeRTOS task as protected
    vTaskDelay(freertos::ticks_t(duration).value());
}

inline void task::notify()
{
    freertos::task::notify_give();
}

inline void task::notify_from_isr()
{
    freertos::task::notify_give_from_isr();
}

inline void task::sleep_periodic(units::milliseconds<size_t> duration)
{
    freertos::task::sleep_periodic(duration);
}

inline bool task::wait_notification(units::milliseconds<size_t> timeout)
{
    freertos::task::notify_take(timeout, false);
    return true;
}

}