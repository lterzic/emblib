#pragma once

#include "details/task_native.hpp"
#include "chrono.hpp"

namespace emblib::rtos {

/**
 * Buffer for static stack allocation
 */
template <size_t SIZE_BYTES>
using task_stack = uint8_t[SIZE_BYTES];

/**
 * Task interface
 */
class task : private details::task_native_t {
public:
    template <size_t STACK_SIZE_BYTES>
    explicit task(const char* name, size_t priority, task_stack<STACK_SIZE_BYTES>& stack);
    virtual ~task() = default;

    /* Copy operations not allowed */
    task(const task&) = delete;
    task& operator=(const task&) = delete;

    /* Move operations not allowed */
    task(task&&) = delete;
    task& operator=(task&&) = delete;

    /**
     * Suspend this task indefinitely
     * @note Only way to have it continue execution is by calling `task::resume`
     */
    void suspend() noexcept;

    /**
     * Allow execution of the task if it was previously suspended
     */
    void resume() noexcept;

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

protected:
    /**
     * Put this task to sleep
     */
    void sleep(ticks duration) noexcept;

    /**
     * Put this task to sleep until (last wake up time from this method + period)
     * @note First time this is called, next wake up time is relative to task creation
     * @todo Can change return type to bool to signal if woke up on time
     */
    void sleep_periodic(ticks period) noexcept;

    /**
     * Wait for this task to get notified, ie. for the task's notification
     * value to become greater than zero
     * @param clear_count If true, task's notification values is cleared rather
     * than decremented
     * @note Lightweight version of a semaphore which can be taken only by this task
     * @returns True if the notification was received before
     * the timeout passed, else false
     */
    bool wait_notification(ticks timeout, bool clear_count = false) noexcept;

private:
    /**
     * Task executable
     */
    virtual void run() noexcept = 0;

};

/**
 * Task with internal stack
 */
template <size_t STACK_SIZE_BYTES>
class static_task : public task {
public:
    explicit static_task(const char* name, size_t priority) :
        task(name, priority, m_stack)
    {}

private:
    task_stack<STACK_SIZE_BYTES> m_stack;
};

}

#include "details/task_impl.hpp"
