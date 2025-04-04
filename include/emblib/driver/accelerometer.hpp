#pragma once

#include "emblib/emblib.hpp"
#include "sensor3d.hpp"

namespace emblib::driver {

class accelerometer : public sensor3d<float> {

public:
    /**
     * Conversion rate from standard gravity to meters per second^2
     */
    static constexpr float G_TO_MPS2 = 9.80665f;

};

}

#if EMBLIB_UNNEST_NAMESPACES
namespace emblib {
    using namespace driver;
}
#endif