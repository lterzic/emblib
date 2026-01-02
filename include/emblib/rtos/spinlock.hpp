#pragma once

#include "lock.hpp"
#include <atomic>

namespace emblib::rtos {

class spinlock {
public:
    spinlock() :
        m_lock(false)
    {}

    /* Copy operations not allowed */
    spinlock(const spinlock&) = delete;
    spinlock& operator=(const spinlock&) = delete;

    /* Move operations not allowed */
    spinlock(spinlock&&) = delete;
    spinlock& operator=(spinlock&&) = delete;

    /**
     * Try to acquire the lock in a loop until successful
     * @todo Add an inner loop to just read without exchange
     * to reduce cache strain on multi core systems
     */
    void lock() noexcept
    {
        while (m_lock.exchange(true, std::memory_order_acquire)) {}
    }

    /**
     * Try to acquire the lock once
     * @returns `true` if successful
     */
    bool try_lock() noexcept
    {
        return !m_lock.exchange(true, std::memory_order_acquire);
    }

    /**
     * Release the lock
     * @note Use only if previosuly acquired the lock. No mechanism
     * is used to check if the owner is correct, so can lead to
     * undefined behavior
     */
    void unlock() noexcept
    {
        m_lock.store(false, std::memory_order_release);
    }

private:
    std::atomic<bool> m_lock;
};

}