#pragma once

#include "types.hpp"
#include <etl/span.h>
#include <cstdint>

namespace emblib::io {

/**
 * Input device
 */
template <typename data_type>
class idev_base {
public:
    /**
     * Read up to `size` elements into the buffer.
     * 
     * @returns Number of bytes read if at least one byte was read
     * at timeout or the entire buffer was filled before timeout. The
     * resulting value should never be 0.
    */
    virtual result read(etl::span<data_type> buffer, timeout timeout) noexcept = 0;

    /**
     * Start an asynchronous read.
     * 
     * Returns immediately after trying to starting the read operation.
     * If successful, the operation will call the callback at some point,
     * otherwise the callback will not be executed.
     * 
     * @todo Return a handle of the async operation to allow canceling
     * a single operation if there are multiple running in parallel.
     */
    virtual etl::expected<void, error> read_async(etl::span<data_type> buffer, async_cb cb) noexcept
    {
        cb(read(buffer, timeout::MAX));
        return {};
    }

    /**
     * Abort currently active read operation.
     * 
     * @return If `true` was returned, the `read_async` operation was
     * stopped correctly and the provided callback should be called with
     * the `ABORT` error. If `false` is returned, either the async operation
     * couldn't be stopped or there wasn't one active.
     * 
     * @note Default implementation returns `false` since the default
     * implementation of the `read_async` is the synchronous, and will
     * only return once the operation is complete.
     */
    virtual bool abort_async_read() noexcept
    {
        return false;
    }
};

/**
 * Input device
 */
using idev = idev_base<uint8_t>;

}