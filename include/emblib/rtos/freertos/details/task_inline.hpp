#pragma once

template <size_t STACK_SIZE_BYTES>
inline task::task(const char *name, size_t priority, task_stack_t<STACK_SIZE_BYTES> &stack) :
    m_native_task([this] {this->run();}, name, priority, (freertos::task_stack_t<sizeof(stack) / sizeof(freertos::task_stack_t<1>)>&)stack)
{
}

inline void task::start_tasks() noexcept
{
    freertos::start_scheduler();
}

inline void task::sleep(milliseconds_t duration) noexcept
{
    vTaskDelay(duration.value());
}

inline void task::sleep_periodic(milliseconds_t period) noexcept
{
    m_native_task.sleep_periodic(period.value());
}

#if EMBLIB_RTOS_SUPPORT_NOTIFICATIONS
inline bool task::wait_notification(milliseconds_t timeout) noexcept
{
    return ulTaskNotifyTake(false, timeout.value());
}

inline void task::notify() noexcept
{
    m_native_task.notify();
}

inline void task::notify_from_isr() noexcept
{
    m_native_task.notify_from_isr();
}
#endif