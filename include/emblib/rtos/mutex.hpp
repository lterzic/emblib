#pragma once

#include "emblib/emblib.hpp"
#if EMBLIB_RTOS_USE_FREERTOS
    #include "./freertos/semaphore.hpp"
#else
#endif
#include "emblib/rtos/task.hpp"

namespace emblib::rtos {

/**
 * Mutex
 * @note Can be used with std guards and locks
 */
class mutex {

public:
#if EMBLIB_RTOS_USE_FREERTOS
    using native_mutex_t = freertos::mutex;
#else
    #error "Thread implementation missing"
#endif

    explicit mutex() = default;

    /* Copy operations not allowed */
    mutex(const mutex&) = delete;
    mutex& operator=(const mutex&) = delete;

    /* Move operations not allowed */
    mutex(mutex&&) = delete;
    mutex& operator=(mutex&&) = delete;

    /**
     * Mutex lock
     * @returns `true` if successful
     */
    bool lock(milliseconds_t timeout = MILLISECONDS_MAX) noexcept;

    /**
     * Mutex unlock
     * @returns `true` if successful
     */
    bool unlock() noexcept;

    /**
     * Get reference to the underlying mutex object
     */
    native_mutex_t& get_native_mutex() noexcept
    {
        return m_native_mutex;
    }

private:
    native_mutex_t m_native_mutex;

};

class scoped_lock {
public:
    explicit scoped_lock(mutex& mutex) noexcept :
        m_mutex(mutex)
    {
        m_mutex.lock();
    }

    ~scoped_lock() noexcept
    {
        m_mutex.unlock();
    }

private:
    mutex& m_mutex;
};


#if EMBLIB_RTOS_USE_FREERTOS
    #include "./freertos/details/mutex_inline.hpp"
#else
#error "Mutex implementation missing"
#endif

}

#if EMBLIB_UNNEST_NAMESPACES
namespace emblib {
    using namespace rtos;
}
#endif