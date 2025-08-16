#pragma once

#include "emblib/units/time.hpp"
#include "details/queue_native.hpp"

namespace emblib::rtos {

using namespace emblib::units;

/**
 * Thread-safe FIFO queue
 */
template <typename item_type, size_t CAPACITY>
class queue : private details::queue_native_t<item_type, CAPACITY> {

public:
    explicit queue() = default;

    /* Copy operations not allowed */
    queue(const queue&) = delete;
    queue& operator=(const queue&) = delete;

    /* Move operations not allowed */
    queue(queue&&) = delete;
    queue& operator=(queue&&) = delete;

    /**
     * Send item to the queue
     * @returns `false` on timeout, else `true`
     */
    bool send(const item_type& item, milliseconds<size_t> timeout) noexcept;

    /**
     * Send item to the queue, don't block if queue is full
     * @note Same behavior as using `send` with timeout = 0
     */
    bool send_from_isr(const item_type& item) noexcept;

    /**
     * Receive item from the queue
     * @returns `false` on timeout, else `true`
     */
    bool receive(item_type& buffer, milliseconds<size_t> timeout) noexcept;

    /**
     * Similar to receive, but doesn't remove the item from the queue
     */
    bool peek(item_type& buffer, milliseconds<size_t> timeout) noexcept;

};

}

#include "details/queue_impl.hpp"
