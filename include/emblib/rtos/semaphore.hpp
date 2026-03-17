#pragma once

#include "chrono.hpp"
#include <emblib/rtos/backend/semaphore.hpp>
#include <type_traits>

namespace emblib::rtos {

/**
 * Semaphore API.
 */
template <typename semaphore_type>
using is_semaphore = std::conjunction<
    /**
     * Semaphore is required to support default construction.
     * Counting semaphore constructor can optionally be supported.
     */
    std::is_constructible<semaphore_type>,

    /**
     * Wait on a signal and decrement once available.
     * @returns Whether the signal was received within the given timeout.
     */
    std::is_invocable_r<bool, decltype(&semaphore_type::wait), semaphore_type&, timeout>,

    /**
     * Signal on a given semaphore.
     * @returns False if the semaphore was already at the highest count,
     * else increments it and returns true.
     * @note Semaphore API can optionally provide a separate method
     * to signal from an ISR context.
     */
    std::is_invocable_r<bool, decltype(&semaphore_type::signal), semaphore_type&>
>;

static_assert(is_semaphore<semaphore>::value);

}