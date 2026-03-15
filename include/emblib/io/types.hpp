#pragma once

#include <etl/delegate.h>
#include <etl/expected.h>
#include <chrono>

namespace emblib::io {

/**
 * Types of errors that can occur during IO operations.
 */
enum class error {
    TIMEOUT,
    BUSY,
    INVAL,
    IO,
    ABORT
};

/**
 * Result of an IO operation.
 * 
 * In case the operation was successful, number of bytes written or
 * read is returned. Otherwise, the type of error is returned.
 * 
 * @note Successful value should always be greater than 0.
 */
using result = etl::expected<size_t, error>;

/**
 * Async IO operation callback.
 * 
 * If the async operation starts, it will execute the callback whether
 * it finishes successfully, or fails during the operation. The result
 * is then passed as the callback argument.
 */
using async_cb = etl::delegate<void (result)>;

/**
 * Timeout specifies the unit of time during which the IO operation
 * is permitted to run once it has started.
 */
using timeout = std::chrono::duration<size_t, std::milli>;

}