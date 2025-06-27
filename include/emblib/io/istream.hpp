#pragma once

#include "emblib/emblib.hpp"
#include "iodef.hpp"

namespace emblib::io {

/**
 * Generic input device interface
 */
template <typename data_type = char>
class istream {

public:
    /**
     * Read up to `size` bytes into the buffer
     * 
     * If `timeout` is 0, the function exits as soon as it tried
     * to read any data from the device. If no data was available, the
     * return value should be 0, else the number of bytes that was
     * available and read is returned. A positive timeout value
     * means that the function should wait up to that time to read
     * all `size` bytes and will return sooner if all the bytes are ready.
     * The max timeout value represents waiting infinitely for all of
     * the data to be read.
     * 
     * @return If a negative value was returned, there was an error
     * during the read operation and it can't be assumed if any data
     * was read. A non-negative return value indicates that there
     * was no error and represents the number of bytes successfully read.
    */
    virtual ssize_t read(data_type* buffer, size_t size, timeout_t timeout) noexcept = 0;

    /**
     * Start an async read
     * 
     * Returns immediately after trying to start the operation and
     * returns true if it was started successfully. The callback
     * should only be called if the start was okay. There is no defined
     * timeout for the operation to complete, to manually stop the
     * operation, see `abort_async`.
     * 
     * Callback status argument represents the number of bytes read
     * during the operation. If negative value was returned, there was
     * an error and nothing can be assumed about the number of bytes
     * read.
     * 
     * @note Default implementation is a synchronous `read` with timeout
     * set to 0
     */
    virtual bool read_async(data_type* buffer, size_t size, callback_t cb) noexcept
    {
        cb(read(buffer, size, timeout_t(0)));
        return true;
    }

    /**
     * Abort currently active read operation
     * 
     * @note Default implementation is doing nothing since the default
     * implementation of the `read_async` is the synchronouse `read`
     */
    virtual bool abort_async_read() noexcept
    {
        return true;
    }
};

}