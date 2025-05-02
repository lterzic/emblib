#pragma once

#include "emblib/emblib.hpp"
#include "emblib/utils/units.hpp"

namespace emblib::driver {

class servo {

public:
    using degrees_t = units::unit_t<degree_t, float>;

public:
    explicit servo() = default;
    virtual ~servo() = default;

    /**
     * Set the desired position (angle) for the servo
     * @note Should be between min and max angles
     */
    virtual bool write_angle(degrees_t angle) noexcept = 0;

    /**
     * Read the current servo position (angle)
     */
    virtual bool read_angle(degrees_t& angle) noexcept = 0;

    /**
     * Get the minimum angle
     */
    virtual degrees_t get_min_angle() const noexcept = 0;

    /**
     * Get the maximum angle
     */
    virtual degrees_t get_max_angle() const noexcept = 0;
};

}

#if EMBLIB_UNNEST_NAMESPACES
namespace emblib {
    using namespace driver;
}
#endif