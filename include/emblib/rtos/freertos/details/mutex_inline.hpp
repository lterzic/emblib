#pragma once

inline bool mutex::lock(milliseconds_t timeout) noexcept
{
    return m_native_mutex.take(timeout.value());
}

inline bool mutex::unlock() noexcept
{
    return m_native_mutex.give();
}