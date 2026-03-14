#pragma once

#include "iodev.hpp"
#include "types.hpp"

namespace emblib::io {

/**
 * @todo If needed replace this with a class i2c_address which
 * can then support both 7 and 10 bit addresses
 */
using i2c_address = uint16_t;

/**
 * I2C master
 * 
 * @note Behaviour of the API is the same as for `idev` and `odev` with the
 * difference of each method requiring an address. For more detailed description
 * refer to those interfaces.
 */
class i2c_bus {
public:
    /**
     * Read up to `size` bytes into the buffer from the device with the specified address.
     */
    virtual result read(i2c_address address, etl::span<uint8_t> buffer, timeout timeout) noexcept = 0;

    /**
     * Write a span of bytes to the device with the specified address.
     */
    virtual result write(i2c_address address, etl::span<const uint8_t> data, timeout timeout) noexcept = 0;

    /**
     * Read up to `size` bytes into the buffer from the device with the specified address.
     */
    virtual etl::expected<void, error> read_async(i2c_address address, etl::span<uint8_t> buffer, async_cb cb) noexcept
    {
        cb(read(address, buffer, timeout::MAX));
        return {};
    }

    /**
     * Write a span of bytes to the device with the specified address.
     */
    virtual etl::expected<void, error> write_async(i2c_address address, etl::span<const uint8_t> data, async_cb cb) noexcept
    {
        cb(write(address, data, timeout::MAX));
        return {};
    }

    /**
     * Abort a started async read operation.
     */
    virtual bool abort_async_read() noexcept
    {
        return false;
    }

    /**
     * Abort a started async write operation.
     */
    virtual bool abort_async_write() noexcept
    {
        return false;
    }
};

/**
 * I2C slave
 */
class i2c_dev : public iodev {
public:
    i2c_dev(i2c_bus& bus, i2c_address address) :
        m_bus(bus),
        m_address(address)
    {}

    result read(etl::span<uint8_t> buffer, timeout timeout) noexcept override
    {
        return m_bus.read(m_address, buffer, timeout);
    }

    etl::expected<void, error> read_async(etl::span<uint8_t> buffer, async_cb cb) noexcept override
    {
        return m_bus.read_async(m_address, buffer, cb);
    }

    bool abort_async_read() noexcept override
    {
        return m_bus.abort_async_read();
    }

    result write(etl::span<const uint8_t> data, timeout timeout) noexcept override
    {
        return m_bus.write(m_address, data, timeout);
    }

    etl::expected<void, error> write_async(etl::span<const uint8_t> data, async_cb cb) noexcept override
    {
        return m_bus.write_async(m_address, data, cb);
    }

    bool abort_async_write() noexcept override
    {
        return m_bus.abort_async_write();
    }

private:
    i2c_bus& m_bus;
    i2c_address m_address;
};

}