#pragma once

#include "emblib/emblib.hpp"

#if EMBLIB_RTOS_USE_FREERTOS
    #include "emblib/rtos/freertos/queue.hpp"
#endif

namespace emblib::rtos::details {

#if EMBLIB_RTOS_USE_FREERTOS
    template <typename item_type, size_t CAPACITY>
    using queue_native_t = freertos::queue<item_type, CAPACITY>;
#endif

}