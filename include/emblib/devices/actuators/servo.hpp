#pragma once

#include "emblib/units/angle.hpp"

namespace emblib::devices {

using namespace emblib::units;

class servo {

public:
    explicit servo() = default;
    virtual ~servo() = default;

    /**
     * Set the desired position (angle) for the servo
     * @note Should be between min and max angles
     */
    virtual bool write_angle(radians<float> angle) noexcept = 0;

    /**
     * Read the current servo position (angle)
     */
    virtual bool read_angle(radians<float>& angle) noexcept = 0;

    /**
     * Get the minimum angle
     */
    virtual radians<float> get_min_angle() const noexcept = 0;

    /**
     * Get the maximum angle
     */
    virtual radians<float> get_max_angle() const noexcept = 0;
};

}
