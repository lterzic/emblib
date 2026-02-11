#pragma once

#include <atomic>
#include <cassert>
#include <cstddef>

namespace emblib::lockfree {

/**
 * Lock-free Single Producer Multi Consumer queue
 */
template <typename item_type, size_t CAPACITY>
class spmc_queue {
    static_assert(std::is_trivially_copyable_v<item_type>);
    static_assert(std::is_trivially_destructible_v<item_type>);
public:
    spmc_queue() :
        m_alloc_ptr(0),
        m_write_ptr(0)
    {}

    spmc_queue(const spmc_queue&) = delete;
    spmc_queue& operator=(const spmc_queue&) = delete;
    spmc_queue(spmc_queue&&) = delete;
    spmc_queue& operator=(spmc_queue&&) = delete;

    /**
     * Construct the item in-place in the queue.
     */
    template<typename... Args>
    void emplace(Args&&... args) noexcept
    {
        m_alloc_ptr++;
        new (get_slot(m_write_ptr % CAPACITY)) item_type{std::forward<Args>(args)...};
        m_write_ptr++;
    }

    /**
     * Push an item by copy.
     */
    void push(const item_type& item) noexcept
    {
        emplace(item);
    }

    /**
     * Get a reader of this queue.
     */
    template <bool CHECK_OVERFLOW>
    auto get_reader(bool from_current = false) const noexcept
    {
        return reader<CHECK_OVERFLOW>(*this, from_current);
    }

    /**
     * Get the all-time count of items added to this queue.
     */
    size_t get_total_count() const noexcept
    {
        return m_write_ptr.load();
    }

private:
    item_type* get_slot(size_t idx) noexcept {
        return reinterpret_cast<item_type*>(m_buffer + idx * sizeof(item_type));
    }

    template <bool CHECK_OVERFLOW>
    class reader {
    public:
        explicit reader(const spmc_queue& queue, bool from_current) :
            m_queue(queue),
            m_read_ptr(from_current ? queue.m_write_ptr.load() : 0)
        {}

        /**
         * Read an item if available.
         * 
         * @note If `CHECK_OVERFLOW` is false, this function
         * asserts that no overflow has happened.
         */
        bool read(item_type& item_buffer) noexcept
        {
            if (m_read_ptr == m_queue.m_write_ptr)
                return false;

            if constexpr(CHECK_OVERFLOW) {
                // We assert because we can't recover
                assert(!has_overflowed());
            }

            item_buffer = get_item(m_read_ptr % CAPACITY);
            m_read_ptr++;
            return true;
        }

        /**
         * Check if the queue has overflowed, and the item
         * we were supposed to read was overwritten.
         */
        bool has_overflowed() const noexcept
        {
            return m_queue.m_alloc_ptr > m_read_ptr + CAPACITY;
        }
    
    private:
        /**
         * Get a copy of a queue item.
         */
        auto get_item(size_t idx) noexcept {
            return *reinterpret_cast<const item_type*>(m_queue.m_buffer + idx * sizeof(item_type));
        }
    
    private:
        const spmc_queue& m_queue;
        std::atomic<size_t> m_read_ptr;
    };

private:
    alignas(item_type) uint8_t m_buffer[sizeof(item_type) * CAPACITY];
    
    std::atomic<size_t> m_alloc_ptr;
    std::atomic<size_t> m_write_ptr;
};

}