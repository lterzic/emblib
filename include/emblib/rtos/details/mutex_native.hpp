#pragma once

#include "emblib/emblib.hpp"

#if EMBLIB_RTOS_USE_FREERTOS
    #include "emblib/rtos/freertos/semaphore.hpp"
#endif

namespace emblib::rtos::details {

#if EMBLIB_RTOS_USE_FREERTOS
    using mutex_native_t = freertos::mutex;
#endif

}