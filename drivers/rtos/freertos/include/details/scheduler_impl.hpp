#pragma once

#include "emblib/rtos/freertos/scheduler.hpp"
#include "emblib/rtos/scheduler.hpp"

namespace emblib::rtos {

static inline void start_scheduler() noexcept
{
    freertos::start_scheduler();
}

/**
 * Is the CPU currently managed by a scheduler
 */
static inline bool is_scheduler_running() noexcept
{
    return freertos::get_scheduler_state() == freertos::scheduler_state_e::RUNNING;
}

}