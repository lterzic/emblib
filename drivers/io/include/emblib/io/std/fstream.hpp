#pragma once

#include "emblib/io/iostream.hpp"
#include <fstream>

namespace emblib::io::std {

class fstream : public iostream {
public:
    explicit fstream(::std::fstream& fstream) :
        m_fstream(fstream)
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
    ::std::fstream& m_fstream;
};

}