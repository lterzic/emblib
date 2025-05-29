#pragma once

#include "emblib/rtos/freertos/semaphore.hpp"

namespace emblib::rtos::details {

using mutex_native_t = freertos::mutex;

}