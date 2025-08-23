#pragma once

namespace emblib::rtos {

/**
 * Start the scheduler
 */
static inline void start_scheduler() noexcept;

/**
 * Is the CPU currently managed by a scheduler
 */
static inline bool is_scheduler_running() noexcept;

}

#include "details/scheduler_impl.hpp"