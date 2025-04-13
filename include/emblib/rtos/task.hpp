#pragma once

#include "emblib/emblib.hpp"
#if EMBLIB_RTOS_USE_FREERTOS
    #include "./freertos/task.hpp"
#else
#endif
#include "emblib/utils/chrono.hpp"

namespace emblib::rtos {

/**
 * Statically allocated stack
 */
template <size_t SIZE_IN_BYTES>
using task_stack_t = uint8_t[SIZE_IN_BYTES];

/**
 * Thread interface
 */
class task {

public:
#if EMBLIB_RTOS_USE_FREERTOS
    using native_task_t = freertos::task;
#else
    #error "Task implementation missing"
#endif

public:
    template <size_t STACK_SIZE_BYTES>
    explicit task(
        const char* name,
        size_t priority,
        task_stack_t<STACK_SIZE_BYTES>& stack
    );
    virtual ~task() = default;

    /* Copy operations not allowed */
    task(const task&) = delete;
    task& operator=(const task&) = delete;

    /* Move operations not allowed */
    task(task&&) = delete;
    task& operator=(task&&) = delete;

    /**
     * Start the scheduler
     */
    static inline void start_tasks() noexcept;

    /**
     * Put the currently running thread to sleep
     * @note Static since can be called even baremetal and implemented using HAL
     */
    static inline void sleep(milliseconds_t duration) noexcept;

#if EMBLIB_RTOS_SUPPORT_NOTIFICATIONS
    /**
     * Increment this task's notification value
     * @note Unblocks this task if is currently waiting on notification
     */
    void notify() noexcept;

    /**
     * Increment this task's notification value from ISR
     * @note Unblocks this task if is currently waiting on notification
     */
    void notify_from_isr() noexcept;
#endif

    /**
     * Get reference to the underlying implementation object
     */
    native_task_t& get_native_task() noexcept
    {
        return m_native_task;
    }

protected:
    /**
     * Put this task to sleep until (last wake up time from this method + period)
     * @note First time this is called, next wake up time is relative to task creation
     * @todo Can change return type to bool to signal if woke up on time
     */
    void sleep_periodic(milliseconds_t period) noexcept;

#if EMBLIB_RTOS_SUPPORT_NOTIFICATIONS
    /**
     * Wait for this task to get notified
     * @note Lightweight version of a semaphore which can be
     * taken only by this task
     */
    bool wait_notification(milliseconds_t timeout = MILLISECONDS_MAX) noexcept;
#endif

private:
    /**
     * Task executable
     */
    virtual void run() noexcept = 0;

private:
    native_task_t m_native_task;

};


#if EMBLIB_RTOS_USE_FREERTOS
    #include "./freertos/details/task_inline.hpp"
#else
    #error "Thread implementation missing"
#endif

}

#if EMBLIB_UNNEST_NAMESPACES
namespace emblib {
    using namespace rtos;
}
#endif