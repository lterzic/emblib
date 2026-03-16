#pragma once

#include "chrono.hpp"
#include <emblib/rtos/backend/queue.hpp>
#include <type_traits>

namespace emblib::rtos {

/**
 * Queue API.
 */
template <template <typename item_type, size_t CAPACITY> typename queue_type, typename T, size_t C>
using is_queue = std::conjunction<
    std::is_constructible<queue_type<T, C>>,

    /**
     * Place an item into the queue.
     * @return True if the item was placed within the given timeout, else
     * false, usually because the queue was full the entire duration.
     */
    std::is_invocable_r<bool, decltype(&queue_type<T, C>::send), queue_type<T, C>&, const T&, timeout>,

    /**
     * Get an item from the queue, and remove it.
     * @return True if the item was taken within the timeout.
     */
    std::is_invocable_r<bool, decltype(&queue_type<T, C>::receive), queue_type<T, C>&, T&, timeout>
>;

static_assert(is_queue<queue, int, 0>::value);

}