#pragma once

#include <emblib/io/iodev.hpp>
#include <atomic>
#include <condition_variable>
#include <mutex>
#include <thread>

namespace emblib::posix {

/**
 * Implementation of the IO device interface using POSIX sockets.
 *
 * @note This object is not owning meaning it expects an already open
 * file descriptor and it is up to the user to close it.
 *
 * Provides synchronous read/write with timeout, and asynchronous read
 * backed by a persistent background thread that can be aborted.
 */
class sock_dev : public io::iodev {
public:
    explicit sock_dev(int fd) noexcept;
    ~sock_dev() noexcept;

    // Non-copyable, non-movable (has std::thread member)
    sock_dev(const sock_dev&) = delete;
    sock_dev& operator=(const sock_dev&) = delete;
    sock_dev(sock_dev&&) = delete;
    sock_dev& operator=(sock_dev&&) = delete;

    /**
     * Blocking receive with a given timeout.
     */
    io::result read(etl::span<uint8_t> buffer, io::timeout timeout) noexcept override;

    /**
     * Non-blocking receive with an infinite timeout.
     * Submits a job to the background thread. Returns error::BUSY if
     * an async read is already in progress.
     */
    etl::expected<void, io::error> read_async(etl::span<uint8_t> buffer, io::async_cb cb) noexcept override;

    /**
     * Abort an active async read.
     * Returns true if an async read was active and aborted; false otherwise.
     */
    bool abort_async_read() noexcept override;

    /**
     * Blocking send with a given timeout.
     */
    io::result write(etl::span<const uint8_t> data, io::timeout timeout) noexcept override;

    /** Returns the underlying file descriptor. */
    int fd() const noexcept { return m_fd; }

private:
    /**
     * Async read thread which waits in a loop for read operations.
     */
    void async_thread_fn() noexcept;

private:
    int m_fd;
    int m_pipe_fds[2]; // [0]=read-end, [1]=write-end

    bool m_shutdown = false;
    std::atomic<bool> m_active{false};
    std::atomic<bool> m_abort{false};
    etl::span<uint8_t> m_async_buf;
    io::async_cb m_async_cb;

    std::mutex m_async_mtx;
    std::condition_variable m_async_cv;
    std::thread m_async_thread; // must be last member
};

}