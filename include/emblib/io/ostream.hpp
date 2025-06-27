#pragma once

#include "emblib/emblib.hpp"
#include "emblib/units/si.hpp"
#include <etl/delegate.h>

namespace emblib::driver {

/**
 * Generic output device interface
 */
template <typename data_type = char>
class ostream {

public:
    /**
     * Async write result callback
     */
    using callback_t = etl::delegate<void(ssize_t)>;

    /**
     * Blocking operation timeout in milliseconds
     * @note Set to `-1` for maximum (infinite) timeout
     */
    using timeout_t = units::milliseconds<size_t>;

public:
    /**
     * Write an array of bytes to the device
     * 
     * If `timeout` is 0, the function exits as soon as it tried
     * to write any data to the device. It's possible that all of the
     * data was written on the first try, but it's also possible
     * that none of the data was written. The max timeout value
     * represents waiting infinitely for all of the data to be written.
     * 
     * @return If a negative value was returned, there was an error
     * during the write operation and it can't be assumed if any data
     * was written. A non-negative return value indicates that there
     * was no error and represents the number of bytes successfully
     * written before the timeout period passed.
    */
    virtual ssize_t write(const data_type* data, size_t size, timeout_t timeout) noexcept = 0;

    /**
     * Start an async write
     * 
     * Returns immediately after trying to start the operation and
     * returns true if it was started successfully. The callback
     * should only be called if the start was okay. There is no defined
     * timeout for the operation to complete, to manually stop the
     * operation, see `abort_async`.
     * 
     * Callback status argument represents the number of bytes written
     * during the operation. If negative value was returned, there was
     * an error and nothing can be assumed about the number of bytes
     * written.
     * 
     * @note Default implementation is a synchronous `write` with timeout
     * set to 0
     */
    virtual bool write_async(const char* data, size_t size, callback_t cb) noexcept
    {
        return cb(write(data, size, timeout_t(0)));
    }

    /**
     * Abort currently active write operation
     * 
     * @note Default implementation is doing nothing since the default
     * implementation of the `write_async` is the synchronouse `write`
     */
    virtual bool abort_async_write() noexcept
    {
        return true;
    }
};

}