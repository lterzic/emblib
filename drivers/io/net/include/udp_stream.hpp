#pragma once

#include <emblib/io/istream.hpp>
#include <emblib/io/ostream.hpp>
#include <arpa/inet.h>
#include <future>

namespace emblib::io::net {

class udp_stream : public emblib::io::istream, public emblib::io::ostream {

public:
    explicit udp_stream(const char* send_ip, int send_port, int bind_port = -1);
    virtual ~udp_stream();
    
    ssize_t write(const char* data, size_t size, emblib::io::timeout_t timeout) noexcept override;
    ssize_t read(char* data, size_t size, emblib::io::timeout_t timeout) noexcept override;

    bool read_async(char* data, size_t size, emblib::io::callback_t callback) noexcept override;
    bool abort_async_read() noexcept override;

private:
    int m_socket_fd;
    int m_abort_fd;
    sockaddr_in m_send_endpoint;
    sockaddr_in m_recv_endpoint;

    std::future<void> m_read_future;
};

}