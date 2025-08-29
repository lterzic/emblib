#pragma once

#include "emblib/io/iostream.hpp"
#include <iostream>

namespace emblib::io::std {

class iostream : public emblib::io::iostream {
public:
    explicit iostream(::std::iostream& iostream) :
        m_iostream(iostream)
    {}

    ssize_t read(char* buffer, size_t size, timeout_t timeout) noexcept override
    {
        (void)timeout;
        
        m_iostream.read(buffer, size);
        return m_iostream.gcount();
    }

    ssize_t write(const char* data, size_t size, timeout_t timeout) noexcept override
    {
        (void)timeout;

        m_iostream.write(data, size);
        return size;
    }

    bidir_mode_e get_bidir_mode() const noexcept override
    {
        return bidir_mode_e::HALF_DUPLEX;
    }

private:
    ::std::iostream& m_iostream;
};

}