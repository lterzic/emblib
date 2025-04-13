#pragma once

#include "i2c_bus.hpp"
#include "emblib/emblib.hpp"
#include <etl/vector.h>

namespace emblib::driver {

/**
 * @note Can also extends i2c_bus to allow i2c master
 * operations to the last selected output channel
 */
class i2c_mux {

public:
    explicit i2c_mux(i2c_bus& parent, size_t channel_count) :
        m_parent(parent), m_channel_count(channel_count), m_current_index(-1)
    {
        assert(channel_count <= I2C_MUX_MAX_CHANNELS);
        /* Initialize all the output channel wrapper classes */
        for (size_t i = 0; i < m_channel_count; i++) {
            m_channels.emplace_back(*this, i);
        }
    }
    virtual ~i2c_mux() = default;

    /**
     * Get a reference (pointer) to a wrapper i2c master class
     * for the output channel with the given index (starting with 0)
     */
    i2c_bus* get_channel(size_t ch_index) const noexcept
    {
        if (ch_index >= m_channel_count) {
            return nullptr;
        }
        return &const_cast<i2c_mux_channel&>(m_channels[ch_index]);
    }

    /**
     * Number of output channels for this mux
     */
    size_t get_channel_count() const noexcept
    {
        return m_channel_count;
    }

private:
    /**
     * Select output channel
     * @note Calls handler if needed
     */
    bool select_channel(size_t index) noexcept
    {
        if (index >= m_channel_count) {
            return false;
        }

        if (m_current_index == index) {
            return true;
        }

        if (handle_select_channel(index)) {
            m_current_index = index;
            return true;
        }
        return false;
    }

    /**
     * Product specific implementation of selecting the output channel
     * @returns `true` if channel selected successfully
     * @note Input is validated before this is called
     */
    virtual bool handle_select_channel(size_t index) noexcept = 0;

private:
    /**
     * Wrapper class for enabling looking at output channels as separate i2c bus masters
     */
    class i2c_mux_channel : public i2c_bus {

    public:
        explicit i2c_mux_channel(i2c_mux& mux, size_t index) :
            m_mux(mux), m_index(index) {}

        ssize_t write(i2c_address_t address, const char* data, size_t size, milliseconds_t timeout = MILLISECONDS_MAX) noexcept override
        {
            if (!m_mux.select_channel(m_index)) {
                return -1;
            }
            return m_mux.m_parent.write(address, data, size, timeout);
        }

        ssize_t read(i2c_address_t address, char* buffer, size_t size, milliseconds_t timeout = MILLISECONDS_MAX) noexcept override
        {
            if (!m_mux.select_channel(m_index)) {
                return -1;
            }
            return m_mux.m_parent.read(address, buffer, size, timeout);
        }

        bool write_async(i2c_address_t address, const char* data, size_t size, const callback_t cb = callback_t()) noexcept override
        {
            if (!m_mux.select_channel(m_index)) {
                return -1;
            }
            return m_mux.m_parent.write_async(address, data, size, cb);
        }

        bool read_async(i2c_address_t address, char* buffer, size_t size, const callback_t cb = callback_t()) noexcept
        {
            if (!m_mux.select_channel(m_index)) {
                return -1;
            }
            return m_mux.m_parent.read_async(address, buffer, size, cb);
        }

    private:
        i2c_mux& m_mux;
        size_t m_index;
    };

private:
    etl::vector<i2c_mux_channel, I2C_MUX_MAX_CHANNELS> m_channels;
    size_t m_channel_count;
    ssize_t m_current_index;

    i2c_bus& m_parent;

};

}

#if EMBLIB_UNNEST_NAMESPACES
namespace emblib {
    using namespace driver;
}
#endif