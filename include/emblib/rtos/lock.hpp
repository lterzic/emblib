#pragma once

#include <etl/tuple.h>

namespace emblib::rtos {

/**
 * Mutex lock which lasts from creation until the end of the
 * scope, with dead-lock prevention.
 */
template<typename... lock_types>
class scoped_lock {
public:
    explicit scoped_lock(lock_types&... locks) :
        m_locks(locks...)
    {
        (locks.lock(), ...);
    }

    scoped_lock(const scoped_lock&) = delete;
    scoped_lock& operator=(const scoped_lock&) = delete;
    scoped_lock(scoped_lock&&) = delete;
    scoped_lock& operator=(scoped_lock&&) = delete;

    ~scoped_lock()
    {
        unlock(etl::index_sequence_for<lock_types...>{});
    }

private:
    template<size_t... INDICES>
    void unlock(etl::index_sequence<INDICES...>) {
        (..., etl::get<sizeof...(INDICES) - 1 - INDICES>(m_locks).unlock());
    }

private:
    etl::tuple<lock_types&...> m_locks;
};

}