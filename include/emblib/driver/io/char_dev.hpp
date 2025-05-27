#pragma once

#include "emblib/emblib.hpp"
#include "emblib/utils/chrono.hpp"
#include <etl/delegate.h>

namespace emblib::driver {

/**
 * Base class for all objects which want to provide an interface
 * for writing and reading character (byte) streams
 * @todo Rename to io_dev
 * @todo Support read and write offsets
 */
class char_dev {

public:
    /* Typedef of callback functions for async operations */
    using callback_t = etl::delegate<void(ssize_t)>;
    /**
     * Blocking operation timeout in milliseconds
     * @note Set to `-1` for maximum (infinite) timeout,
     * behavior undefined for other negative values
     */
    using timeout_t = milliseconds_t;

    explicit char_dev() = default;
    virtual ~char_dev() = default;

    /* Copy operations not allowed */
    char_dev(const char_dev&) = delete;
    char_dev& operator=(const char_dev&) = delete;

    /* Move operations not allowed */
    char_dev(char_dev&&) = delete;
    char_dev& operator=(char_dev&&) = delete;

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
    virtual ssize_t write(const char* data, size_t size, timeout_t timeout) noexcept = 0;

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
    virtual ssize_t read(char* buffer, size_t size, timeout_t timeout) noexcept = 0;

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
     */
    virtual bool write_async(const char* data, size_t size, const callback_t cb = callback_t()) noexcept
    {
        return false;
    }

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
     */
    virtual bool read_async(char* buffer, size_t size, const callback_t cb = callback_t()) noexcept
    {
        return false;
    }

    /**
     * Abort the current async operation (write/read)
     * @todo Can be replaced with a `reset` method
     */
    virtual void abort_async() noexcept
    {
        // No implementation by default as async operations
        // are not required
    }

    /**
     * @return `true` if this char device supports async operations
     */
    virtual bool is_async_available() noexcept
    {
        return false;
    }

    /**
     * Tests if the device is responding
     * @returns `true` if device responds
     * @note Default implementation is a dummy read
    */
    virtual bool probe(timeout_t timeout) noexcept
    {
        return read(nullptr, 0, timeout) == 0;
    }

};

}

#if EMBLIB_UNNEST_NAMESPACES
namespace emblib {
    using namespace driver;
}
#endif