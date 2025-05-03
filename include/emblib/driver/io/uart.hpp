#pragma once

#include "emblib/emblib.hpp"
#include "char_dev.hpp"

namespace emblib::driver {

struct uart_config_s {
    uart::baud_rate_e baud_rate;
    uart::data_bits_e data_bits;
    uart::stop_bits_e stop_bits;
    uart::parity_bit_e parity;
};

class uart : public char_dev {

public:
    enum class baud_rate_e : uint32_t {
        BR9600      = 9600,
        BR115200    = 115200
    };

    enum class data_bits_e : uint8_t {
        DB7 = 7,
        DB8 = 8,
        DB9 = 9
    };

    enum class stop_bits_e : uint8_t {
        SB1 = 1,
        SB2 = 2
    };

    enum class parity_bit_e {
        NONE,
        EVEN,
        ODD
    };

};

}

#if EMBLIB_UNNEST_NAMESPACES
namespace emblib {
    using namespace driver;
}
#endif