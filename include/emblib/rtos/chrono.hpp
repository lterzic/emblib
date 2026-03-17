#pragma once

#include <chrono>

namespace emblib::rtos {

/**
 * Milliseconds is the default timeout type for RTOS APIs.
 */
using timeout = std::chrono::duration<size_t, std::milli>;

}