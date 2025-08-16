#pragma once

#include "emblib/io/iodef.hpp"

namespace emblib::driver {

/**
 * @todo If needed replace this with a class i2c_address which
 * can then support both 7 and 10 bit addresses
 */
typedef uint16_t i2c_address_t;

/**
 * I2C master base class
 * 
 * @todo Currently not supporting 10 bit addresses, but can
 * add this property to constructor parameters
 */
class i2c_bus {

public:
    explicit i2c_bus() = default;
    virtual ~i2c_bus() = default;

    /* Copy operations not allowed */
    i2c_bus(const i2c_bus&) = delete;
    i2c_bus& operator=(const i2c_bus&) = delete;

    /* Move operations not allowed */
    i2c_bus(i2c_bus&&) = delete;
    i2c_bus& operator=(i2c_bus&&) = delete;

    /**
     * Write an array of bytes to the device with the specified address
    */
    virtual ssize_t write(i2c_address_t address, const char* data, size_t size, io::timeout_t timeout) noexcept = 0;

    /**
     * Read up to `size` bytes into the buffer from the device with the specified address
    */
    virtual ssize_t read(i2c_address_t address, char* buffer, size_t size, io::timeout_t timeout) noexcept = 0;

    /**
     * Start an async write
     */
    virtual bool write_async(i2c_address_t address, const char* data, size_t size, io::callback_t cb) noexcept
    {
        cb(write(address, data, size, io::timeout_t(0)));
        return true;
    }

    /**
     * Start an async read
     */
    virtual bool read_async(i2c_address_t address, char* buffer, size_t size, io::callback_t cb) noexcept
    {
        cb(read(address, buffer, size, io::timeout_t(0)));
        return true;
    }

    /**
     * Abort the current async operation (write/read)
     */
    virtual void abort_async() noexcept
    {
        reset();
    }

    /**
     * Tests if the device is responding
     * @returns `true` if device responds
     * @note Default implementation is a dummy read
    */
    virtual bool ping(i2c_address_t address, io::timeout_t timeout) noexcept
    {
        return read(address, nullptr, 0, timeout) == 0;
    }

    /**
     * Resets the I2C master object state and releases the bus if it is held
     * @note Constructors should use this function to initialize the device
    */
    virtual bool reset() = 0;

};

}

#if EMBLIB_UNNEST_NAMESPACES
namespace emblib {
    using namespace driver;
}
#endif