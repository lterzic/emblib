#include <emblib/posix/sock_dev.hpp>

#include <fcntl.h>
#include <poll.h>
#include <sys/socket.h>
#include <unistd.h>

namespace emblib::posix {

static int
to_poll_ms(io::timeout t) noexcept
{
    if (t == io::timeout::min())
        return 0;
    if (t == io::timeout::max())
        return -1;
    return static_cast<int>(t.count());
}

sock_dev::sock_dev(int fd) noexcept :
    m_fd(fd)
{
    // Create abort pipe for signaling the async thread
    ::pipe(m_pipe_fds);

    // Set both pipe ends non-blocking to avoid blocking on read/write
    ::fcntl(m_pipe_fds[0], F_SETFL, ::fcntl(m_pipe_fds[0], F_GETFL) | O_NONBLOCK);
    ::fcntl(m_pipe_fds[1], F_SETFL, ::fcntl(m_pipe_fds[1], F_GETFL) | O_NONBLOCK);

    m_async_thread = std::thread([this] { async_thread_fn(); });
}

sock_dev::~sock_dev() noexcept
{
    {
        std::lock_guard lock(m_async_mtx);
        m_shutdown = true;
    }
    m_async_cv.notify_one();
    m_async_thread.join();

    ::close(m_pipe_fds[0]);
    ::close(m_pipe_fds[1]);
}

io::result
sock_dev::read(etl::span<uint8_t> buffer, io::timeout timeout) noexcept
{
    if (buffer.empty()) {
        return etl::unexpected{io::error::INVAL};
    }

    ::pollfd pfd{m_fd, POLLIN, 0};
    int ret = ::poll(&pfd, 1, to_poll_ms(timeout));

    if (ret == 0)
        return etl::unexpected{io::error::TIMEOUT};
    if (ret < 0)
        return etl::unexpected{io::error::IO};

    ssize_t n = ::recv(m_fd, buffer.data(), buffer.size(), 0);
    if (n <= 0)
        return etl::unexpected{io::error::IO};

    return static_cast<size_t>(n);
}

io::result
sock_dev::write(etl::span<const uint8_t> data, io::timeout timeout) noexcept
{
    if (data.empty()) {
        return etl::unexpected{io::error::INVAL};
    }

    ::pollfd pfd{m_fd, POLLOUT, 0};
    int ret = ::poll(&pfd, 1, to_poll_ms(timeout));

    if (ret == 0)
        return etl::unexpected{io::error::TIMEOUT};
    if (ret < 0)
        return etl::unexpected{io::error::IO};

    ssize_t n = ::send(m_fd, data.data(), data.size(), MSG_NOSIGNAL);
    if (n <= 0)
        return etl::unexpected{io::error::IO};

    return static_cast<size_t>(n);
}

etl::expected<void, io::error>
sock_dev::read_async(etl::span<uint8_t> buffer, io::async_cb cb) noexcept
{
    if (buffer.empty()) {
        return etl::unexpected{io::error::INVAL};
    }

    std::lock_guard lock(m_async_mtx);

    // Only allow one active async read at a time
    if (m_active.load(std::memory_order_relaxed)) {
        return etl::unexpected{io::error::BUSY};
    }

    m_async_buf = buffer;
    m_async_cb = cb;

    m_active.store(true, std::memory_order_relaxed);
    m_async_cv.notify_one();

    return {};
}

bool sock_dev::abort_async_read() noexcept
{
    if (!m_active.load(std::memory_order_acquire)) {
        return false;
    }

    m_abort.store(true, std::memory_order_release);
    uint8_t byte = 1;
    ::write(m_pipe_fds[1], &byte, 1);

    return true;
}

void sock_dev::async_thread_fn() noexcept
{
    while (true) {
        {
            std::unique_lock lock(m_async_mtx);
            m_async_cv.wait(lock, [this] {
                return m_shutdown ||
                       m_active.load(std::memory_order_relaxed);
            });
        }

        if (m_shutdown) {
            return;
        }

        ::pollfd fds[2];
        fds[0] = {m_fd, POLLIN, 0};
        fds[1] = {m_pipe_fds[0], POLLIN, 0};

        io::result result{etl::unexpected{io::error::IO}};
        int ret = ::poll(fds, 2, -1);

        if (fds[1].revents & POLLIN) {
            uint8_t drain;
            ::read(m_pipe_fds[0], &drain, 1);
        }

        if (m_abort.exchange(false, std::memory_order_acq_rel)) {
            result = etl::unexpected{io::error::ABORT};
        } else if (ret > 0 && (fds[0].revents & POLLIN)) {
            ssize_t n = ::recv(m_fd, m_async_buf.data(), m_async_buf.size(), 0);
            if (n > 0)
                result = static_cast<size_t>(n);
        }

        io::async_cb cb;
        {
            std::lock_guard lock(m_async_mtx);
            cb = m_async_cb;
            m_active.store(false, std::memory_order_release);
        }
        cb(result);
    }
}

}
