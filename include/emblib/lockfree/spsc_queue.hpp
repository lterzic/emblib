#pragma once

#include <atomic>
#include <cstddef>

namespace emblib::lockfree {

/**
 * Lock-free thread safe Single Producer Single Consumer queue
 */
template <typename item_type, size_t CAPACITY>
class spsc_queue {
    static_assert(std::is_trivially_copyable_v<item_type>);
    static_assert(std::is_trivially_destructible_v<item_type>);
    
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
     * Construct the item in-place in the queue if the queue
     * is not full.
     */
    template<typename... Args>
    bool emplace(Args&&... args) noexcept
    {
        size_t old_tail = m_tail.load(std::memory_order_relaxed);
        size_t next_tail = inc_loop(old_tail);

        // If the tail caught up with the head of the queue, it's full
        if (next_tail == m_head.load(std::memory_order_acquire)) {
            return false;
        }

        new (get_slot(old_tail)) item_type{std::forward<Args>(args)...};
        m_tail.store(next_tail, std::memory_order_release);
        return true;
    }

    /**
     * Push an item into the queue if the queue is not full.
     */
    bool push(const item_type& item) noexcept
    {
        return emplace(item);
    }

    /**
     * Pop
     */
    bool pop(item_type& item_buffer) noexcept
    {
        size_t head = m_head.load(std::memory_order_relaxed);
        if (head == m_tail.load(std::memory_order_acquire)) {
            return false;
        }

        item_buffer = *get_slot(head);
        m_head.store(inc_loop(head), std::memory_order_release);
        return true;
    }

private:
    item_type* get_slot(size_t idx) noexcept {
        return reinterpret_cast<item_type*>(m_buffer + idx * sizeof(item_type));
    }

    static size_t inc_loop(size_t idx) noexcept
    {
        return ++idx == BUFFER_SIZE ? 0 : idx;
    }

private:
    alignas(item_type) uint8_t m_buffer[sizeof(item_type) * BUFFER_SIZE];

    std::atomic<size_t> m_head;
    std::atomic<size_t> m_tail;
};

}