#pragma once

#include "emblib/emblib.hpp"

#if EMBLIB_RTOS_USE_FREERTOS
    #include "emblib/rtos/freertos/task.hpp"
#endif

namespace emblib::rtos {

#if EMBLIB_RTOS_USE_FREERTOS
    using task_native_t = freertos::task;
#endif

}