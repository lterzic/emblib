#pragma once

#include "emblib/units/time.hpp"
#include <FreeRTOS.h>
#include <task.h>

namespace emblib::rtos::freertos {

/**
 * Ticks are defined relative to 1 second based on the FreeRTOS config parameter.
 */
using ticks = ::units::unit_t<
    ::units::unit<std::ratio<1, configTICK_RATE_HZ>, units::details::seconds_unit>,
    unsigned int
>;

/**
 * Get the number of ticks since system start.
 */
static inline ticks get_ticks() noexcept
{
    return ticks(xTaskGetTickCount());
}

}