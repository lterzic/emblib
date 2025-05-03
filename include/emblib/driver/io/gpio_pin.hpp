#pragma once

#include "emblib/emblib.hpp"
#include <functional>

namespace emblib::driver {

class gpio_pin {

public:
    enum class mode_e {INPUT, OUTPUT, OUTPUT_OD, BIDIR, BIDIR_OD};
    enum class pull_e {NONE, UP, DOWN};
    enum class intr_e {NONE, RISING, FALLING, BOTH};

public:
    explicit gpio_pin() = default;
    virtual ~gpio_pin() = default;

    // Copy operations not allowed
    gpio_pin(const gpio_pin&) = delete;
    gpio_pin& operator=(const gpio_pin&) = delete;

    // Move operations not allowed
    gpio_pin(gpio_pin&&) = delete;
    gpio_pin& operator=(gpio_pin&&) = delete;

    /**
     * Read the current state of the pin into the user provided buffer
     * @returns `true` if the `state` value now has valid data
     */
    virtual bool read(bool& state) noexcept = 0;

    /**
     * Write the given state to the pin
     * @returns `true` for a successful write
     */
    virtual bool write(bool state) noexcept = 0;
    
    /**
     * Toggle the current state
     * @returns `true` if toggled successfully
     */
    virtual bool toggle() noexcept = 0;

    /**
     * Set the pin mode
     */
    virtual bool set_mode(mode_e mode) noexcept = 0;

    /**
     * Set the pull resistor type
     */
    virtual bool set_pull(pull_e pull) noexcept = 0;

    /**
     * Set the interrupt trigger type and the callback
     */
    virtual bool set_intr(intr_e intr, std::function<void()>) noexcept = 0;

};

}

#if EMBLIB_UNNEST_NAMESPACES
namespace emblib {
    using namespace driver;
}
#endif