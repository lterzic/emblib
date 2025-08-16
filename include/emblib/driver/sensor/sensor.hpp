#pragma once

#include "emblib/units/time.hpp"

namespace emblib::driver {

template <typename data_type>
class sensor {

public:
    /**
     * Public data_type accessor
     */
    using data_t = data_type;

    /**
     * Type definition for unit of number of events (samples) per second
     */
    using rate_t = units::hertz<size_t>;

public:
    /**
     * Default constructor and destructor
    */
    explicit sensor() = default;
    virtual ~sensor() = default;

    /**
     * Get the name of the sensor
     * @todo Return type can be `const etl::istring&`
     */
    virtual const char* get_name() const noexcept = 0;

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
     * Noise spectral density
     * @note Return units are assumed to be [data_type]/sqrt(Hz)
     */
    virtual float get_noise_density() const noexcept = 0;
};

}

#if EMBLIB_UNNEST_NAMESPACES
namespace emblib {
    using namespace driver;
}
#endif