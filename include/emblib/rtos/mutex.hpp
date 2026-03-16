#pragma once

#include "chrono.hpp"
#include <emblib/rtos/backend/mutex.hpp>
#include <type_traits>

namespace emblib::rtos {

/**
 * Mutex API. Can be used with std locks.
 */
template <typename mutex_type>
using is_mutex = std::conjunction<
    std::is_constructible<mutex_type>,

    /**
     * Try to lock the mutex.
     * @returns Whether the mutex was successfully locked within the given timeout.
     */
    std::is_invocable_r<bool, decltype(&mutex_type::lock), mutex_type&, timeout>,

    /**
     * Unlock the mutex. This assumes that the calling thread has previously
     * successfully locked it.
     */
    std::is_invocable_r<void, decltype(&mutex_type::unlock), mutex_type&>
>;

static_assert(is_mutex<mutex>::value);

}