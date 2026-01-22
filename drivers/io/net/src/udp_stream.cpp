#include "emblib/io/net/udp_stream.hpp"
#include <arpa/inet.h>
#include <iostream>
#include <sys/poll.h>
#include <sys/eventfd.h>
#include <sys/socket.h>
#include <poll.h>
#include <unistd.h>

namespace emblib::io::net {

udp_stream::udp_stream(const char* send_ip, int send_port, int bind_port)
{
    m_socket_fd = socket(AF_INET, SOCK_DGRAM, 0);
    m_abort_fd = eventfd(0, EFD_NONBLOCK);

    m_send_endpoint.sin_family = AF_INET;
    m_send_endpoint.sin_addr.s_addr = inet_addr(send_ip);
    m_send_endpoint.sin_port = htons(send_port);

    m_recv_endpoint.sin_family = AF_INET;
    m_recv_endpoint.sin_addr.s_addr = inet_addr("127.0.0.1");
    m_recv_endpoint.sin_port = htons(bind_port);

    int optval = 1;
    assert(setsockopt(m_socket_fd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) >= 0);

    if (bind_port != -1)
        assert(bind(m_socket_fd, (sockaddr*)&m_recv_endpoint, sizeof(m_recv_endpoint)) >= 0);
}

udp_stream::~udp_stream()
{
    close(m_socket_fd);
    close(m_abort_fd);
}

ssize_t udp_stream::write(const char* data, size_t size, emblib::io::timeout_t timeout) noexcept
{
    return sendto(m_socket_fd, data, size, 0, (sockaddr*)&m_send_endpoint, sizeof(m_send_endpoint));
}

ssize_t udp_stream::read(char* data, size_t size, emblib::io::timeout_t timeout) noexcept
{
    timeval sock_timeout;
    sock_timeout.tv_sec = timeout.value() / 1000;
    sock_timeout.tv_usec = (timeout.value() % 1000) * 1000;
    assert(setsockopt(m_socket_fd, SOL_SOCKET, SO_RCVTIMEO, &sock_timeout, sizeof(sock_timeout)) >= 0);

    return recvfrom(m_socket_fd, data, size, 0, NULL, 0);
}

bool udp_stream::read_async(char* data, size_t size, emblib::io::callback_t callback) noexcept
{
    m_read_future = std::async(std::launch::async, [this, data, size, callback]{
        pollfd fds[2];
        fds[0].fd = m_socket_fd;
        fds[0].events = POLLIN;
        fds[1].fd = m_abort_fd;
        fds[1].events = POLLIN;

        // Clear abort event in case someone called abort while there
        // was no active read
        uint64_t v;
        ::read(m_abort_fd, &v, sizeof(v));

        // Wait forever on one of two events
        int ret = poll(fds, 2, -1);
        if (ret < 0) {
            callback(-1);
            return;
        }

        if (fds[0].revents & POLLIN) {
            ssize_t status = recvfrom(m_socket_fd, data, size, 0, NULL, 0);
            callback(status);
            return;
        }

        if (fds[1].revents & POLLIN) {
            ::read(m_abort_fd, &v, sizeof(v));
            callback(-1);
            return;
        }
    });
    return true;
}

bool udp_stream::abort_async_read() noexcept
{
    uint64_t v = 1;
    ::write(m_abort_fd, &v, sizeof(v));
    return true;
}

}