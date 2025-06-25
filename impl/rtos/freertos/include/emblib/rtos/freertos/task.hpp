#pragma once

#include "emblib/emblib.hpp"
#include "emblib/utils/chrono.hpp"
#include <FreeRTOS.h>
#include <task.h>

namespace emblib::rtos::freertos {

/**
 * Possible FreeRTOS scheduler states
 */
enum class scheduler_state_e {
    SUSPENDED   = taskSCHEDULER_SUSPENDED,
    NOT_STARTED = taskSCHEDULER_NOT_STARTED,
    RUNNING     = taskSCHEDULER_RUNNING,
};

/**
 * Start FreeRTOS scheduler
*/
static inline void start_scheduler() noexcept
{
    vTaskStartScheduler();
}

/**
 * Return the scheduler state
 */
static inline scheduler_state_e get_scheduler_state() noexcept
{
    return static_cast<scheduler_state_e>(xTaskGetSchedulerState());
}

/**
 * Stack buffer for a FreeRTOS task, where `SIZE` is the
 * number of words for the allocation
 */
template <size_t SIZE_WORDS>
using task_stack_t = StackType_t[SIZE_WORDS];

/**
 * Tick as the time unit defined with respect to the FreeRTOS config parameter
 */
using ticks_t = ::units::unit_t<
    ::units::unit<std::ratio<1, configTICK_RATE_HZ>, units::details::seconds_unit>,
    unsigned int
>;

/**
 * FreeRTOS Task
 */
class task {

public:
    template <size_t STACK_SIZE_WORDS>
    explicit task(
        const char* name,
        size_t priority,
        task_stack_t<STACK_SIZE_WORDS>& stack
    ) :
        m_task_handle(xTaskCreateStatic(
            reinterpret_cast<void (*)(void*)>(&task_entry),
            name,
            STACK_SIZE_WORDS,
            this,
            priority,
            stack,
            &m_task_buffer
        ))
    {}

    /* Copy operations not allowed */
    task(const task&) = delete;
    task& operator=(const task&) = delete;

    /* Move operations not allowed */
    task(task&&) = delete;
    task& operator=(task&&) = delete;

    /**
     * Increment task's notification value (works like a counting semaphore)
     */
    void notify_give() noexcept
    {
        xTaskNotifyGive(m_task_handle);
    }

    /**
     * Increment task's notification value
     * @todo Add argument for higher priority task woken
     */
    void notify_give_from_isr() noexcept
    {
        vTaskNotifyGiveFromISR(m_task_handle, NULL);
    }

protected:
    /**
     * Wait for a notification to this task
     * @todo Return the notification count before it is decremented (cleared)
     */
    void notify_take(ticks_t timeout, bool clear_count = false) noexcept
    {
        ulTaskNotifyTake(clear_count, timeout.value());
    }

    /**
     * Sleep relative to previous wake up time
     * @returns `true` if wakeup was delayed, false if woke up on time
     */
    bool sleep_periodic(ticks_t period) noexcept
    {
        if (m_first_period) {
            m_first_period = false;
            m_prev_wakeup = xTaskGetTickCount();
        }
        return xTaskDelayUntil(&m_prev_wakeup, period.value()) == pdTRUE;
    }

private:
    /**
     * Task function
     */
    virtual void run() noexcept = 0;

    /**
     * Actual function which is called by the scheduler which then calls the
     * appropriate cpp style task function to allow for passing contexts to
     * the thread
     */
    static void task_entry(task* instance) noexcept
    {
        instance->run();

        /* If the function ever exits, remove this task */
        vTaskDelete(instance->m_task_handle);
    }

private:
    StaticTask_t m_task_buffer;
    TaskHandle_t m_task_handle;

    TickType_t m_prev_wakeup;
    bool m_first_period = true;
};

}