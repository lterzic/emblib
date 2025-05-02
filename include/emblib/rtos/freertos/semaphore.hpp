#pragma once

#include "emblib/emblib.hpp"
#include "task.hpp"
#include <FreeRTOS.h>
#include <semphr.h>

namespace emblib::rtos::freertos {

/**
 * Possible FreeRTOS semaphore implementations
 */
enum class semaphore_type_e {
    BINARY,
    COUNTING,
    MUTEX
};

/**
 * FreeRTOS semaphore
 */
template <semaphore_type_e TYPE = semaphore_type_e::BINARY>
class semaphore {

public:
    template <semaphore_type_e type = TYPE, typename = std::enable_if_t<type != semaphore_type_e::COUNTING>>
    explicit semaphore() noexcept :
        m_semaphore_handle(
            TYPE == semaphore_type_e::MUTEX ?
            xSemaphoreCreateMutexStatic(&m_semaphore_buffer) :
            xSemaphoreCreateBinaryStatic(&m_semaphore_buffer))
    {}

    template <semaphore_type_e type = TYPE, typename = std::enable_if_t<type == semaphore_type_e::COUNTING>>
    explicit semaphore(size_t max_count, size_t initial_count = 0) noexcept :
        m_semaphore_handle(xSemaphoreCreateCountingStatic(max_count, initial_count, &m_semaphore_buffer))
    {}

    /* Copy operations not allowed */
    semaphore(const semaphore&) = delete;
    semaphore& operator=(const semaphore&) = delete;

    /* Move operations not allowed */
    semaphore(semaphore&&) = delete;
    semaphore& operator=(semaphore&&) = delete;

    /**
     * Semaphore take
     * @todo Can add checking if the scheduler has started and returning
     * `true` if not since that means there can be only 1 thread running
    */
    bool take(ticks_t ticks) noexcept
    {
        return xSemaphoreTake(m_semaphore_handle, ticks.value()) == pdTRUE;
    }

    /**
     * Semaphore give
     * @todo Similar to `take`, can return true if scheduler not started
    */
    bool give() noexcept
    {
        return xSemaphoreGive(m_semaphore_handle) == pdTRUE;
    }

    /**
     * Semaphore give from interrupt routine
     * @note Use this to make sure that task which was
     * interrupted does not block accidentally
     * @todo Can add higher prio task woken
     */
    bool give_from_isr() noexcept
    {
        return xSemaphoreGiveFromISR(m_semaphore_handle, NULL) == pdTRUE;
    }

private:
    StaticSemaphore_t m_semaphore_buffer;
    SemaphoreHandle_t m_semaphore_handle;

};

using mutex = semaphore<semaphore_type_e::MUTEX>;

}