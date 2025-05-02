#pragma once

#include "emblib/emblib.hpp"
#include "sensor3d.hpp"
#include "emblib/utils/units.hpp"

namespace emblib::driver {

class gyroscope : public sensor3d<units::unit_t<radian_per_second_t, float>> {

};

}

#if EMBLIB_UNNEST_NAMESPACES
namespace emblib {
    using namespace driver;
}
#endif