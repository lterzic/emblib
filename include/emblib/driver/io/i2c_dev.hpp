#pragma once

#include "emblib/io/istream.hpp"
#include "emblib/io/ostream.hpp"
#include "i2c_bus.hpp"

namespace emblib::driver {

/**
 * I2C device
 */
class i2c_dev final : public io::istream<char>, public io::ostream<char> {

public:
    explicit i2c_dev(i2c_bus& i2c_bus, i2c_address_t address)
        : m_i2c_bus(i2c_bus), m_address(address) {}

    /**
     * Write an array of bytes to this device
    */
    ssize_t write(const char* data, size_t size, io::timeout_t timeout) noexcept override
    {
        return m_i2c_bus.write(m_address, data, size, timeout);
    }

    /**
     * Read up to `size` bytes into the buffer
    */
    ssize_t read(char* buffer, size_t size, io::timeout_t timeout) noexcept override
    {
        return m_i2c_bus.read(m_address, buffer, size, timeout);
    }

    /**
     * Start an async write
     * @return `true` if write operation started successfully
     */
    bool write_async(const char* data, size_t size, io::callback_t cb) noexcept override
    {
        return m_i2c_bus.write_async(m_address, data, size, cb);
    }

    /**
     * Start an async read
     * @return `true` if write operation started successfully
     */
    bool read_async(char* buffer, size_t size, io::callback_t cb) noexcept override
    {
        return m_i2c_bus.read_async(m_address, buffer, size, cb);
    }

    bool abort_async_write() noexcept override
    {
        m_i2c_bus.abort_async();
        return true;
    }

    bool abort_async_read() noexcept override
    {
        m_i2c_bus.abort_async();
        return true;
    }

private:
    i2c_address_t m_address;
    i2c_bus& m_i2c_bus;
};

}
