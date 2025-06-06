#pragma once

#include "emblib/emblib.hpp"
#include "emblib/utils/units.hpp"

namespace emblib::driver {

template <typename data_type>
class sensor {

public:
    /**
     * Type definition for unit of number of events (samples) per second
     */
    using rate_t = units::unit_t<units::inverse<second_t>, size_t>;

public:
    /**
     * Default constructor and destructor
    */
    explicit sensor() = default;
    virtual ~sensor() = default;

    /**
     * Check if the sensor identifies correctly
     * @note For external devices this could be a read of the ID (WHO_AM_I)
     * register of the device and comparing it to the expected value
     * @return `true` if register read successfully and data matches
     */
    virtual bool probe() noexcept = 0;

    /**
     * Set the data output rate of the sensor
     * @return Returned value is the rate of the sensor after this
     * method has finished configuring the sensor, regardless of
     * the success status of the method
     */
    virtual rate_t set_rate(rate_t rate) noexcept = 0;

    /**
     * Get the current data output rate
     */
    virtual rate_t get_rate() const noexcept = 0;

    /**
     * Is there new data available since the last read
     */
    virtual bool is_ready() noexcept = 0;    

    /**
     * Read data
     */
    virtual bool read(data_type& out) noexcept = 0;

    /**
     * Noise spectral density in units [data_type]/sqrt(Hz)
     * @todo Create a unit type for the return type based on the data type
     */
    virtual float get_noise_density() const noexcept = 0;
};

}

#if EMBLIB_UNNEST_NAMESPACES
namespace emblib {
    using namespace driver;
}
#endif