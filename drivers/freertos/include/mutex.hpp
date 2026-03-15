#pragma once

#include "semaphore.hpp"

namespace emblib::freertos {

/**
 * FreeRTOS mutex
 * 
 * @note This is just a convenience wrapper of a semaphore.
 */
class mutex : private semaphore {
public:
    mutex() :
        semaphore(0)
    {}

    /**
     * Lock this mutex.
     */
    bool lock(ticks timeout = ticks::max()) noexcept
    {
        return take(timeout);
    }

    /**
     * Unlock this mutex if this thread is the owner.
     */
    bool unlock() noexcept
    {
        return give();
    }
};

}