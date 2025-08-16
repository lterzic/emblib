#pragma once

#include "emblib/rtos/freertos/semaphore.hpp"

namespace emblib::rtos::details {

using semaphore_native_t = freertos::semaphore;

}