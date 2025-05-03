#pragma once

#include "emblib/emblib.hpp"
#include "sensor.hpp"

namespace emblib::driver {

template <typename axis_data_type>
class sensor3d : public sensor<axis_data_type[3]> {

public:
    /* Used for read axis method */
    enum class axis_e {
        X, Y, Z
    };

public:
    /**
     * Default constructor and destructor
    */
    explicit sensor3d() = default;
    virtual ~sensor3d() = default;

    /**
     * Read data of a single axis
     */
    virtual bool read_axis(axis_e axis, axis_data_type& out) noexcept = 0;

    /**
     * Read data of all axes into an array
     * @note Default implementation is to read all 3 axes one by one, but
     * can be overriden to allow reading of all the values in one bus transaction
     */
    virtual bool read(axis_data_type (&out_data)[3]) override noexcept
    {
        bool success = true;
        success &= read_axis(axis_e::X, out_data[(int)axis_e::X]);
        success &= read_axis(axis_e::Y, out_data[(int)axis_e::Y]);
        success &= read_axis(axis_e::Z, out_data[(int)axis_e::Z]);
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