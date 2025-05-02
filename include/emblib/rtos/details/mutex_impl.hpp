#pragma once

#include "emblib/emblib.hpp"

#if EMBLIB_RTOS_USE_FREERTOS
    #include "emblib/rtos/freertos/details/mutex_impl.hpp"
#endif
