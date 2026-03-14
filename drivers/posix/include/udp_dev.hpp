#pragma once

#include <emblib/posix/sock_dev.hpp>

namespace emblib::posix {

/**
 * Owning UDP socket device.
 *
 * Creates a SOCK_DGRAM socket, optionally binds to a local port, connects
 * to a remote endpoint, and inherits all IO behaviour from sock_dev.
 * The socket is closed in the destructor.
 */
class udp_dev : public sock_dev {
public:
    /**
     * Create a UDP socket connected to the given remote host:port.
     *
     * @param remote_host  Remote hostname or dotted-decimal IP string.
     * @param remote_port  Remote UDP port.
     * @param local_port   Local port to bind (0 = OS-assigned ephemeral port).
     */
    udp_dev(const char* remote_host, uint16_t remote_port, uint16_t local_port = 0) noexcept;
    ~udp_dev() noexcept;

    // Non-copyable, non-movable
    udp_dev(const udp_dev&) = delete;
    udp_dev& operator=(const udp_dev&) = delete;
    udp_dev(udp_dev&&) = delete;
    udp_dev& operator=(udp_dev&&) = delete;

private:
    /**
     * Creates the UDP socket, binds, and connects.
     * Returns the fd on success or -1 on failure.
     */
    static int open(const char* remote_host, uint16_t remote_port, uint16_t local_port) noexcept;
};

}
