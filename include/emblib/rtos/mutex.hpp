#pragma once

#include "emblib/emblib.hpp"
#include "emblib/rtos/task.hpp"
#include "details/mutex_native.hpp"

namespace emblib::rtos {

/**
 * Mutex
 * @note Can be used with std guards and locks
 */
class mutex : private details::mutex_native_t {

public:
    explicit mutex() = default;

    /* Copy operations not allowed */
    mutex(const mutex&) = delete;
    mutex& operator=(const mutex&) = delete;

    /* Move operations not allowed */
    mutex(mutex&&) = delete;
    mutex& operator=(mutex&&) = delete;

    /**
     * Try to lock the mutex within the given timeout. If successful
     * returns true, else false.
     */
    bool lock(milliseconds_t timeout = MILLISECONDS_MAX) noexcept;

    /**
     * Try to unlock the mutex. It will only be successful if
     * this thread was the one holding the mutex.
     */
    bool unlock() noexcept;

};

/**
 * Locks the mutex until the end of this scope
 */
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

}

// Include mutex implementation based on emblib config
#include "details/mutex_impl.hpp"

#if EMBLIB_UNNEST_NAMESPACES
namespace emblib {
    using namespace rtos;
}
#endif