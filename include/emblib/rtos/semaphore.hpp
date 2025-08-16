#pragma once

#include "emblib/units/time.hpp"
#include "details/semaphore_native.hpp"

namespace emblib::rtos {

using namespace emblib::units;

/**
 * Semaphore (Binary or Counting)
 */
class semaphore : private details::semaphore_native_t {

public:
    explicit semaphore(size_t max_count = 1) noexcept
    {
        assert(max_count > 0);
    }

    /* Copy operations not allowed */
    semaphore(const semaphore&) = delete;
    semaphore& operator=(const semaphore&) = delete;

    /* Move operations not allowed */
    semaphore(semaphore&&) = delete;
    semaphore& operator=(semaphore&&) = delete;

    /**
     * Wait for a semaphore token and decrement once available
     */
    bool wait(milliseconds<size_t> timeout = milliseconds<size_t>(-1)) noexcept;

    /**
     * Increment the semaphore count
     * @returns `false` if the semaphore was already at `max_count`, else `true`
     */
    bool signal() noexcept;

    /**
     * Same behaviour as `signal`, but some RTOS implementations don't allow
     * calling the regular `signal` method from ISR context
     */
    bool signal_from_isr() noexcept;

};

}

#include "details/semaphore_impl.hpp"

#if EMBLIB_UNNEST_NAMESPACES
namespace emblib {
    using namespace rtos;
}
#endif