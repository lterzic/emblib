#pragma once

#include "emblib/emblib.hpp"
#if EMBLIB_RTOS_USE_FREERTOS
    #include "./freertos/queue.hpp"
#else
    #error "Thread implementation missing"
#endif
#include "emblib/rtos/task.hpp"

namespace emblib::rtos {

/**
 * Thread-safe FIFO queue with sending and receiving task blocking
 */
template <typename item_type, size_t CAPACITY>
class queue {

public:
#if EMBLIB_RTOS_USE_FREERTOS
    using native_queue_t = freertos::queue<item_type, CAPACITY>;
#else
    #error "Queue implementation missing"
#endif

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
    bool send(const item_type& item, milliseconds_t timeout = MILLISECONDS_MAX) noexcept;

    /**
     * Send item to the queue, don't block if queue full
     */
    bool send_from_isr(const item_type& item) noexcept;

    /**
     * Receive item from the queue
     * @returns `false` on timeout, else `true`
     */
    bool receive(item_type& buffer, milliseconds_t timeout = MILLISECONDS_MAX) noexcept;

    /**
     * Similar to receive, but doesn't remove the item from the queue
     */
    bool peek(item_type& buffer, milliseconds_t timeout = MILLISECONDS_MAX) noexcept;

    /**
     * Get reference to the native queue object
     */
    native_queue_t& get_native_queue() noexcept
    {
        return m_native_queue;
    }

private:
    native_queue_t m_native_queue;

};


#if EMBLIB_RTOS_USE_FREERTOS
    #include "./freertos/details/queue_inline.hpp"
#else
#error "Mutex implementation missing"
#endif

}

#if EMBLIB_UNNEST_NAMESPACES
namespace emblib {
    using namespace rtos;
}
#endif