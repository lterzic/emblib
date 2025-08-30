#pragma once

#include "emblib/io/istream.hpp"
#include "emblib/io/ostream.hpp"
#include <iostream>

namespace emblib::io::std {

class istream : public emblib::io::istream {
public:
    explicit istream(::std::istream& istream) :
        m_istream(istream)
    {}

    ssize_t read(char* buffer, size_t size, timeout_t timeout) noexcept override
    {
        (void)timeout;
        
        m_istream.read(buffer, size);
        return m_istream.gcount();
    }

private:
    ::std::istream& m_istream;
};

class ostream : public emblib::io::ostream {
public:
    explicit ostream(::std::ostream& ostream) :
        m_ostream(ostream)
    {}

    ssize_t write(const char* data, size_t size, timeout_t timeout) noexcept override
    {
        (void)timeout;

        m_ostream.write(data, size);
        return size;
    }

private:
    ::std::ostream& m_ostream;
};

}