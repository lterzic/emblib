#pragma once

#include <atomic>
#include <cstddef>

namespace emblib::lockfree {

/**
 * Lock-free thread safe Single Producer Single Consumer queue
 * @note Not thread-safe if using push from multiple threads or
 * pop from multiple threads
 */
template <typename item_type, size_t CAPACITY>
class spsc_queue {
    /**
     * Extra slot is used to determine if the queue is full
     * without using a counter
     */
    static constexpr size_t BUFFER_SIZE = CAPACITY + 1;

public:
    spsc_queue() :
        m_head(0),
        m_tail(0)
    {}

    spsc_queue(const spsc_queue&) = delete;
    spsc_queue& operator=(const spsc_queue&) = delete;
    spsc_queue(spsc_queue&&) = delete;
    spsc_queue& operator=(spsc_queue&&) = delete;

    /**
     * Push by copy
     */
    bool push(const item_type& item) noexcept
    {
        static_assert(std::is_copy_assignable_v<item_type>);

        return push_generic([&item](item_type* item_buffer) {
            *item_buffer = item;
        });
    }

    /**
     * Push by move
     */
    bool push(item_type&& item) noexcept
    {
        static_assert(std::is_move_assignable_v<item_type>);

        return push_generic([&item](item_type* item_buffer) {
            *item_buffer = std::move(item);
        });
    }

    /**
     * Pop
     */
    bool pop(item_type* item_buffer) noexcept
    {
        static_assert(std::is_copy_assignable_v<item_type>);

        return pop_generic([&item_buffer](const item_type* item) {
            *item_buffer = std::move(*item);
        });
    }

private:
    /**
     * Allocate a slot for the new item if available,
     * call the assignment, and update the read pointer
     */
    template <typename assign_lambda_type>
    bool push_generic(assign_lambda_type&& assign_lambda) noexcept
    {
        size_t old_tail = m_tail.load(std::memory_order_relaxed);
        size_t next_tail = inc_loop(old_tail);

        // If the tail caught up with the head of the queue, it's full
        if (next_tail == m_head.load(std::memory_order_acquire)) {
            return false;
        }

        assign_lambda(reinterpret_cast<item_type*>(&m_buffer[sizeof(item_type) * old_tail]));
        m_tail.store(next_tail, std::memory_order_release);
        return true;
    }

    /**
     * Assign the tail element to a buffer if exists,
     * update the pointers
     */
    template <typename assign_lambda_type>
    bool pop_generic(assign_lambda_type&& assign_lambda) noexcept
    {
        size_t head = m_head.load(std::memory_order_relaxed);
        // Queue is empty
        if (head == m_tail.load(std::memory_order_acquire)) {
            return false;
        }

        assign_lambda(reinterpret_cast<item_type*>(&m_buffer[sizeof(item_type) * head]));
        m_head.store(inc_loop(head), std::memory_order_release);
        return true;
    }

    /**
     * Increment a pointer and loop if needed
     */
    static size_t inc_loop(size_t idx) noexcept
    {
        return ++idx == BUFFER_SIZE ? 0 : idx;
    }

private:
    uint8_t m_buffer[sizeof(item_type) * BUFFER_SIZE];

    std::atomic<size_t> m_head;
    std::atomic<size_t> m_tail;
};

}