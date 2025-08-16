#pragma once

#include "istream.hpp"
#include "ostream.hpp"

namespace emblib::io {

/**
 * Communication stream supporting both input and output with generic data type
 */
template <typename data_type>
class iostream_base : public istream_base<data_type>, public ostream_base<data_type> {
public:
    /**
     * Bidirectional communication mode
     */
    enum class bidir_mode_e {
        // Communication can be only in a single direction at a time
        HALF_DUPLEX,
        // Communication can be in both directions simultaneously
        FULL_DUPLEX
    };

    /**
     * Get stream bidirectional communication mode
     */
    virtual bidir_mode_e get_bidir_mode() const noexcept = 0;
};

/**
 * Input/Output stream with `char` data type
 */
using iostream = iostream_base<char>;

}