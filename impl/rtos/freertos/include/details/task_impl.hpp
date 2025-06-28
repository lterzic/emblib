#pragma once

#include "emblib/rtos/freertos/task.hpp"
#include "emblib/rtos/task.hpp"

namespace emblib::rtos {

template <size_t STACK_SIZE_BYTES>
inline task::task(const char* name, size_t priority, task_stack_t<STACK_SIZE_BYTES>& stack) :
    details::task_native_t(name, priority, (freertos::task_stack_t<sizeof(stack) / sizeof(freertos::task_stack_t<1>)>&)stack)
{}

inline void task::start_scheduler()
{
    freertos::start_scheduler();
}

inline bool task::is_scheduler_running()
{
    return freertos::get_scheduler_state() == freertos::scheduler_state_e::RUNNING;
}

inline void task::sleep(milliseconds<size_t> duration)
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

inline void task::sleep_periodic(milliseconds<size_t> duration)
{
    freertos::task::sleep_periodic(duration);
}

inline bool task::wait_notification(milliseconds<size_t> timeout)
{
    freertos::task::notify_take(timeout, false);
    return true;
}

}