#include <emblib/posix/udp_dev.hpp>

#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

namespace emblib::posix {

int udp_dev::open(const char* remote_host, uint16_t remote_port, uint16_t local_port) noexcept
{
    int fd = ::socket(AF_INET, SOCK_DGRAM, 0);
    if (fd < 0)
        return -1;

    if (local_port != 0) {
        ::sockaddr_in local{};
        local.sin_family = AF_INET;
        local.sin_addr.s_addr = htonl(INADDR_ANY);
        local.sin_port = htons(local_port);
        if (::bind(fd, reinterpret_cast<const ::sockaddr*>(&local), sizeof(local)) < 0) {
            ::close(fd);
            return -1;
        }
    }

    ::addrinfo hints{};
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_DGRAM;

    char port_str[6];
    ::snprintf(port_str, sizeof(port_str), "%u", remote_port);

    ::addrinfo* res = nullptr;
    if (::getaddrinfo(remote_host, port_str, &hints, &res) != 0 || res == nullptr) {
        ::close(fd);
        return -1;
    }

    int ret = ::connect(fd, res->ai_addr, res->ai_addrlen);
    ::freeaddrinfo(res);

    if (ret < 0) {
        ::close(fd);
        return -1;
    }

    return fd;
}

udp_dev::udp_dev(const char* remote_host, uint16_t remote_port, uint16_t local_port) noexcept :
    sock_dev(open(remote_host, remote_port, local_port))
{
    assert(fd() > 0);
}

udp_dev::~udp_dev() noexcept
{
    ::close(fd());
}

}
