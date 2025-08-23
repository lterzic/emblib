#pragma once

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

}