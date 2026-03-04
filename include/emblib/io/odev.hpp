#pragma once

#include "types.hpp"
#include <cstddef>

namespace emblib::io {

/**
 * Output device
 */
template <typename data_type>
class odev_base {
public:
    /**
     * Write the span of data into the device.
     * 
     * @returns Number of bytes written before timeout if that value
     * is greater than 0, else an error is returned.
    */
    virtual result write(etl::span<const data_type> buffer, timeout timeout) noexcept = 0;

    /**
     * Start an asynchronous write.
     * 
     * Returns immediately after trying to starting the write operation.
     * If successful, the operation will call the callback at some point,
     * otherwise the callback will not be executed.
     * 
     * @todo Return a handle of the async operation to allow canceling
     * a single operation if there are multiple running in parallel.
     */
    virtual etl::expected<void, error> write_async(etl::span<const data_type> buffer, async_cb cb) noexcept
    {
        cb(write(buffer, timeout::MAX));
        return {};
    }

    /**
     * Abort currently active write operation.
     * 
     * @return If `true` was returned, the `write_async` operation was
     * stopped correctly and the provided callback should be called with
     * the `ABORT` error. If `false` is returned, either the async operation
     * couldn't be stopped or there wasn't one active.
     * 
     * @note Default implementation returns `false` since the default
     * implementation of the `write_async` is the synchronous, and will
     * only return once the operation is complete.
     */
    virtual bool abort_async_write() noexcept
    {
        return false;
    }
};

/**
 * Output device
 */
using odev = odev_base<uint8_t>;

}