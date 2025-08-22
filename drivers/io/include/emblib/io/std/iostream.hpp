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
        
        m_fstream.read(buffer, size);
        return m_fstream.gcount();
    }

    ssize_t write(const char* data, size_t size, timeout_t timeout) noexcept override
    {
        (void)timeout;

        m_fstream.write(data, size);
        return true;
    }

private:
    ::std::iostream& m_iostream;
};

}