#pragma once

#include "idev.hpp"
#include "odev.hpp"

namespace emblib::io {

/**
 * Input/Output device
 */
template <typename data_type>
class iodev_base : public idev_base<data_type>, public odev_base<data_type> {};

/**
 * Input/Output device
 */
using iodev = iodev_base<uint8_t>;

}