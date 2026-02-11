#pragma once

#include <atomic>
#include <cstddef>
#include <cstdint>

namespace emblib::lockfree {

/**
 * Lock-free thread safe allocation from a pre-allocated buffer
 */
template<typename data_type, size_t CAPACITY>
class allocator {
    union block_u {
        uint8_t data[sizeof(data_type)];
        block_u* next;
    };
    
public:
    allocator() :
        m_available_head(nullptr),
        m_allocated_count(0)
    {
        reset();
    }
    
    allocator(const allocator&) = delete;
    allocator& operator=(const allocator&) = delete;
    allocator(allocator&&) = delete;
    allocator& operator=(allocator&&) = delete;
    
    /**
     * Allocate an empty slot
     * @note Allocated buffer is not initialized
     * @returns Pointer to the allocated memory if there was any available,
     * else `nullptr`
     */
    data_type* alloc() noexcept
    {
        block_u* old_head = m_available_head.load(std::memory_order_acquire);

        while (old_head != nullptr) {
            block_u* new_head = old_head->next;

            // Try to atomically update head pointer
            if (m_available_head.compare_exchange_weak(old_head, new_head,
                                               std::memory_order_release,
                                               std::memory_order_acquire)) {
                m_allocated_count.fetch_add(1, std::memory_order_relaxed);
                return reinterpret_cast<data_type*>(old_head);
            }
            // Compare and swap failed, old_head was updated by compare_exchange_weak
            // Loop will retry with new value
        }
        
        // If old_head (m_available_head) is nullptr, the entire buffer is allocated
        return nullptr;
    }
    
    /**
     * Deallocate a slot
     * @note The provided pointer must originate from the `alloc` method of this
     * object, otherwise undefined behavior
     */
    void dealloc(data_type* ptr) noexcept
    {
        block_u* block = reinterpret_cast<block_u*>(ptr);
        block_u* old_head = m_available_head.load(std::memory_order_acquire);
        
        do {
            block->next = old_head;
            // Try to atomically push block onto free list
        } while (!m_available_head.compare_exchange_weak(
            old_head, block, std::memory_order_release, std::memory_order_acquire
        ));
        
        m_allocated_count.fetch_sub(1, std::memory_order_relaxed);
    }
    
    /**
     * Reset the allocator
     * @note Make sure none of the blocks are in use
     */
    void reset() noexcept
    {
        block_u* blocks = reinterpret_cast<block_u*>(m_buffer);
        
        for (size_t i = 0; i < CAPACITY; ++i) {
            blocks[i].next = (i < CAPACITY - 1) ? &blocks[i + 1] : nullptr;
        }
        
        m_available_head.store(blocks, std::memory_order_release);
        m_allocated_count.store(0, std::memory_order_relaxed);
    }

    /**
     * Get the number of allocated nodes
     */
    size_t get_allocation_count() const noexcept
    {
        return m_allocated_count.load(std::memory_order_relaxed);
    }

    /**
     * Get capacity
     */
    constexpr size_t get_capacity() const noexcept
    {
        return CAPACITY;
    }

private:
    // Pre-allocated m_buffer for all blocks
    alignas(block_u) uint8_t m_buffer[sizeof(block_u) * CAPACITY];
    
    // Lock-free stack head using atomic pointer
    std::atomic<block_u*> m_available_head;
    // Not required - used to fetch allocation count in O(1)
    std::atomic<size_t> m_allocated_count;
};

}