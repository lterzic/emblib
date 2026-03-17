#pragma once

#include "chrono.hpp"
#include <emblib/rtos/backend/task.hpp>
#include <type_traits>

namespace emblib::rtos {

/**
 * Public task API.
 */
template <typename task_type>
using is_task = std::conjunction<
    /**
     * Suspend the task from execution and scheduling. Only way to have it resume
     * execution is by calling `resume`.
     */
    std::is_invocable_r<void, decltype(&task_type::suspend), task_type&>,

    /**
     * Allow the task to be executed.
     */
    std::is_invocable_r<void, decltype(&task_type::resume), task_type&>,

    /**
     * Notify the task.
     * @note Notifications are task private semaphores to which anyone
     * can write (signal), but only the task can read (wait on).
     */
    std::is_invocable_r<void, decltype(&task_type::notify), task_type&>
>;

/**
 * Task protected API.
 */
template <typename task_type>
struct is_task_base : task_type {
    static inline constexpr bool value = std::conjunction_v<
        /**
         * Sleep for a given duration.
         */
        std::is_invocable_r<void, decltype(&is_task_base::sleep), is_task_base&, timeout>,

        /**
         * Wait for a notification for a given duration. Second parameter specifies
         * if the notification counter should be cleared (if true) or just decremented.
         */
        std::is_invocable_r<bool, decltype(&is_task_base::wait_on_notify), is_task_base&, timeout, bool>
    >;
};

static_assert(is_task<task>::value);
static_assert(is_task_base<task>::value);

}