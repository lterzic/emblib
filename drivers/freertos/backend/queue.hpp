#pragma once

#include <emblib/freertos/queue.hpp>

namespace emblib::rtos {

template <typename item_type, size_t CAPACITY>
using queue = freertos::queue<item_type, CAPACITY>;

}