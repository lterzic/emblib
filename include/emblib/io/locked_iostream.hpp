#pragma once

#include "iostream.hpp"
#include "emblib/rtos/mutex.hpp"

namespace emblib::io {

template <typename data_type>
class locked_iostream_base : public iostream_base<data_type> {
public:
    explicit locked_iostream_base(
        istream_base<data_type>& istream,
        ostream_base<data_type>& ostream
    ) :
        m_istream(istream),
        m_ostream(ostream)
    {}

    ssize_t read(data_type* buffer, size_t size, timeout_t timeout) noexcept override
    {
        rtos::scoped_lock lock(m_mutex);
        return m_istream.read(buffer, size, timeout);
    }

    ssize_t write(const data_type* data, size_t size, emblib::io::timeout_t timeout) noexcept override
    {
        rtos::scoped_lock lock(m_mutex);
        return m_ostream.write(data, size, timeout);
    }

    bool read_async(data_type* buffer, size_t size, callback_t cb) noexcept override
    {
        m_mutex.lock();
        m_async_cb = [this, cb](ssize_t status) {
            m_mutex.unlock();
            cb(status);
        };

        if (m_istream.read_async(buffer, size, m_async_cb)) {
            return true;
        }

        m_mutex.unlock();
        return false;
    }

    bool write_async(const data_type* data, size_t size, callback_t cb) noexcept override
    {
        m_mutex.lock();
        m_async_cb = [this, cb](ssize_t status) {
            m_mutex.unlock();
            cb(status);
        };

        if (m_ostream.write_async(data, size, m_async_cb)) {
            return true;
        }

        m_mutex.unlock();
        return false;
    }

    bool abort_async_read() noexcept override
    {
        if (m_istream.abort_async_read()) {
            m_mutex.unlock();
            return true;
        }
        return false;
    }

    bool abort_async_write() noexcept override
    {
        if (m_ostream.abort_async_write()) {
            m_mutex.unlock();
            return true;
        }
        return false;
    }

    bidir_mode_e get_bidir_mode() const noexcept override
    {
        return bidir_mode_e::HALF_DUPLEX;
    }

private:
    istream_base<data_type>& m_istream;
    ostream_base<data_type>& m_ostream;
    rtos::mutex m_mutex;
    callback_t m_async_cb;
};

using locked_iostream = locked_iostream_base<char>;

}