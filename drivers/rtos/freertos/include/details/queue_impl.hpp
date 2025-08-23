#pragma once

#include "emblib/rtos/freertos/queue.hpp"
#include "emblib/rtos/queue.hpp"

namespace emblib::rtos {

template <typename item_type, size_t CAPACITY>
inline bool queue<item_type, CAPACITY>::send(const item_type& item, units::milliseconds<size_t> timeout) noexcept
{
    return freertos::queue<item_type, CAPACITY>::send(&item, timeout);
}

template <typename item_type, size_t CAPACITY>
inline bool queue<item_type, CAPACITY>::send_from_isr(const item_type& item) noexcept
{
    return freertos::queue<item_type, CAPACITY>::send_from_isr(&item);
}

template <typename item_type, size_t CAPACITY>
inline bool queue<item_type, CAPACITY>::receive(item_type& buffer, units::milliseconds<size_t> timeout) noexcept
{
    return freertos::queue<item_type, CAPACITY>::receive(&buffer, timeout);
}

template <typename item_type, size_t CAPACITY>
inline bool queue<item_type, CAPACITY>::peek(item_type& buffer, units::milliseconds<size_t> timeout) noexcept
{
    return freertos::queue<item_type, CAPACITY>::peek(&buffer, timeout);
}

}
