#pragma once

#include "emblib/emblib.hpp"
#include "sensor.hpp"
#include "emblib/math/vector.hpp"

namespace emblib::driver {

template <typename axis_data_type>
class sensor3d : public sensor<vector<axis_data_type, 3>> {

public:
    /**
     * Axis enumerator for selecting when reading single axis
     */
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
    virtual bool read(vector<axis_data_type, 3>& out) noexcept override
    {
        bool success = true;
        success &= read_axis(axis_e::X, out((int)axis_e::X));
        success &= read_axis(axis_e::Y, out((int)axis_e::Y));
        success &= read_axis(axis_e::Z, out((int)axis_e::Z));
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