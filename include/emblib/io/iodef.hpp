#pragma once

#include "emblib/units/si.hpp"
#include <etl/delegate.h>

namespace emblib::io {

/**
 * Async operation result callback
 */
using callback_t = etl::delegate<void(ssize_t)>;

/**
 * Blocking operation timeout in milliseconds
 * @note Set to `-1` for maximum (infinite) timeout
 */
using timeout_t = units::milliseconds<size_t>;

}