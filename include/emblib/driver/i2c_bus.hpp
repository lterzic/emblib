#pragma once

#include "emblib/emblib.hpp"
#include "emblib/utils/chrono.hpp"
#include <functional>

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
    /* Private typedef for async operation callbacks */
    using callback_t = std::function<void(ssize_t)>;

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
     * @returns `-1` if error, else number of bytes written
     * @note Exits once the write operation is complete
    */
    virtual ssize_t write(i2c_address_t address, const char* data, size_t size, milliseconds_t timeout = MILLISECONDS_MAX) noexcept = 0;

    /**
     * Read up to `size` bytes into the buffer from the device with the specified address
     * @returns `-1` if error, else number of bytes read
     * @note Exits once the read operation is complete
    */
    virtual ssize_t read(i2c_address_t address, char* buffer, size_t size, milliseconds_t timeout = MILLISECONDS_MAX) noexcept = 0;

    /**
     * Start an async write
     * @return `true` if write operation started successfully
     */
    virtual bool write_async(i2c_address_t address, const char* data, size_t size, const callback_t cb = callback_t()) noexcept = 0;

    /**
     * Start an async read
     * @return `true` if write operation started successfully
     */
    virtual bool read_async(i2c_address_t address, char* buffer, size_t size, const callback_t cb = callback_t()) noexcept = 0;

    /**
     * Tests if the device is responding
     * @returns `true` if device responds
     * @note Default implementation is a dummy read
    */
    virtual bool probe(i2c_address_t address, milliseconds_t timeout) noexcept
    {
        return read(address, nullptr, 0, timeout) == 0;
    }

    /**
     * @return `true` if current serial device supports async operations
     */
    virtual bool is_async_available() noexcept
    {
        return false;
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