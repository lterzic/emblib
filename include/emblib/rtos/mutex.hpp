#pragma once

#include "emblib/units/time.hpp"
#include "details/mutex_native.hpp"
#include "lock.hpp"

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
    bool lock(units::milliseconds<size_t> timeout = units::milliseconds<size_t>(-1)) noexcept;

    /**
     * Try to unlock the mutex. It will only be successful if
     * this thread was the one holding the mutex.
     */
    bool unlock() noexcept;

};

}

#include "details/mutex_impl.hpp"
