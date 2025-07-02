#pragma once

#include "emblib/emblib.hpp"
#include "sensor.hpp"
#include <etl/array.h>

namespace emblib::driver {

/**
 * N-dimensional sensor
 */
template <typename axis_data_type, size_t DIMENSIONS = 3>
class sensornd : public sensor<etl::array<axis_data_type, DIMENSIONS>> {

public:
    /**
     * Default constructor and destructor
    */
    explicit sensornd() = default;
    virtual ~sensornd() = default;

    /**
     * Read data of a single axis
     */
    virtual bool read_axis(size_t axis, axis_data_type& out) noexcept = 0;

    /**
     * Read data of all axes into an array
     * @note Default implementation is to read all 3 axes one by one, but
     * can be overriden to allow reading of all the values in one bus transaction
     */
    virtual bool read(etl::array<axis_data_type, DIMENSIONS>& out) noexcept override
    {
        bool success = true;
        for (size_t i = 0; i < DIMENSIONS; i++)
            success &= read_axis(i, out[i]);
        return success;
    }

    /// @todo Add public static methods for bias estimation and sphere fitting
};

}

#if EMBLIB_UNNEST_NAMESPACES
namespace emblib {
    using namespace driver;
}
#endif