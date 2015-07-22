// Copyright (c) 2011,2015, Robert Escriva
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
//     * Redistributions of source code must retain the above copyright notice,
//       this list of conditions and the following disclaimer.
//     * Redistributions in binary form must reproduce the above copyright
//       notice, this list of conditions and the following disclaimer in the
//       documentation and/or other materials provided with the distribution.
//     * Neither the name of po6 nor the names of its contributors may be used
//       to endorse or promote products derived from this software without
//       specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.

#ifndef po6_net_socket_h_
#define po6_net_socket_h_

// POSIX
#include <netinet/tcp.h>

// po6
#include <po6/errno.h>
#include <po6/io/fd.h>
#include <po6/net/ipaddr.h>
#include <po6/net/location.h>

namespace po6
{
namespace net
{

class socket : public po6::io::fd
{
    public:
        socket();
        explicit socket(int fd);
        ~socket() throw ();

    public:
        PO6_WARN_UNUSED bool reset(int domain, int type, int protocol);

        PO6_WARN_UNUSED bool bind(const ipaddr& addr, in_port_t port);
        PO6_WARN_UNUSED bool bind(const ipaddr& addr);
        PO6_WARN_UNUSED bool bind(const location& loc);
        PO6_WARN_UNUSED bool connect(const ipaddr& addr, in_port_t port);
        PO6_WARN_UNUSED bool connect(const location& loc);
        PO6_WARN_UNUSED bool listen(int backlog);
        PO6_WARN_UNUSED bool accept(socket* newsock);
        PO6_WARN_UNUSED bool shutdown(int how);

        PO6_WARN_UNUSED bool getpeername(location* loc);
        PO6_WARN_UNUSED bool getsockname(location* loc);

        PO6_WARN_UNUSED bool set_sockopt(int level, int optname,
                                         const void *optval, socklen_t optlen);
        PO6_WARN_UNUSED bool set_reuseaddr();
        PO6_WARN_UNUSED bool set_tcp_nodelay();
        PO6_WARN_UNUSED bool sndbuf(size_t size);
        PO6_WARN_UNUSED bool rcvbuf(size_t size);
        PO6_WARN_UNUSED bool sndlowat(size_t size);
        PO6_WARN_UNUSED bool rcvlowat(size_t size);

        PO6_WARN_UNUSED ssize_t recv(void *buf, size_t len, int flags);
        PO6_WARN_UNUSED ssize_t xrecv(void *buf, size_t len, int flags);
        PO6_WARN_UNUSED ssize_t send(const void *buf, size_t len, int flags);
        PO6_WARN_UNUSED ssize_t xsend(const void *buf, size_t len, int flags);

    public:
        socket& operator = (int f);

    private:
        socket(const socket&);
        socket& operator = (const socket&);
};

} // namespace net
} // namespace po6

#endif // po6_net_socket_h_
