#pragma once

#include "emblib/rtos/freertos/queue.hpp"

namespace emblib::rtos::details {

template <typename item_type, size_t CAPACITY>
using queue_native_t = freertos::queue<item_type, CAPACITY>;

}