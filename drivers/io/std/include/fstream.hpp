#pragma once

#include "emblib/io/istream.hpp"
#include "emblib/io/ostream.hpp"
#include <fstream>

namespace emblib::io::std {

class fstream :
    public emblib::io::istream,
    public emblib::io::ostream {
public:
    fstream(const char* filename) :
        m_fstream(filename)
    {}
    
    virtual ~fstream()
    {
        m_fstream.close();
    }

    ssize_t read(char* buffer, size_t size, timeout_t timeout) noexcept override
    {
        (void)timeout;
        return m_fstream.readsome(buffer, size);
    }

    ssize_t write(const char* data, size_t size, timeout_t timeout) noexcept override
    {
        (void)timeout;

        m_fstream.write(data, size);
        return size;
    }

private:
    ::std::fstream m_fstream;
};

}