#pragma once

#include "char_dev.hpp"
#include "i2c_bus.hpp"
#include "emblib/emblib.hpp"

namespace emblib::driver {

/**
 * I2C device
 */
class i2c_dev final : public char_dev {

public:
    explicit i2c_dev(i2c_bus& i2c_bus, i2c_address_t address)
        : m_i2c_bus(i2c_bus), m_address(address) {}

    /**
     * Write an array of bytes to this device
     * @note Refer to `char_dev::write`
    */
    ssize_t write(const char* data, size_t size, milliseconds_t timeout = MILLISECONDS_MAX) noexcept override
    {
        return m_i2c_bus.write(m_address, data, size, timeout);
    }

    /**
     * Read up to `size` bytes into the buffer
     * @note Refer to `char_dev::read`
    */
    ssize_t read(char* buffer, size_t size, milliseconds_t timeout = MILLISECONDS_MAX) noexcept override
    {
        return m_i2c_bus.read(m_address, buffer, size, timeout);
    }

    /**
     * Start an async write
     * @return `true` if write operation started successfully
     */
    bool write_async(const char* data, size_t size, const callback_t cb = callback_t()) noexcept override
    {
        return m_i2c_bus.write_async(m_address, data, size, cb);
    }

    /**
     * Start an async read
     * @return `true` if write operation started successfully
     */
    bool read_async(char* buffer, size_t size, const callback_t cb = callback_t()) noexcept override
    {
        return m_i2c_bus.read_async(m_address, buffer, size, cb);
    }

    /** @todo Add probe override */

    /**
     * @return `true` if current serial device supports async operations
     */
    bool is_async_available() noexcept override
    {
        return m_i2c_bus.is_async_available();
    }

private:
    i2c_address_t m_address;
    i2c_bus& m_i2c_bus;
};

}

#if EMBLIB_UNNEST_NAMESPACES
namespace emblib {
    using namespace driver;
}
#endif