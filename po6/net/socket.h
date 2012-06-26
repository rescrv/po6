// Copyright (c) 2011, Robert Escriva
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
#include <po6/error.h>
#include <po6/io/fd.h>
#include <po6/net/ipaddr.h>
#include <po6/net/location.h>
#include <po6/noncopyable.h>

namespace po6
{
namespace net
{

class socket : public po6::io::fd
{
    public:
        socket();
        socket(int domain, int type, int protocol);
        ~socket() throw ();

    public:
        void bind(const ipaddr& addr, in_port_t port);
        void bind(const ipaddr& addr);
        void bind(const location& loc);
        void connect(const ipaddr& addr, in_port_t port);
        void connect(const location& loc);
        void listen(int backlog);
        void accept(socket* newsock);
        void shutdown(int how);
        void reset(int domain, int type, int protocol);

        location getpeername();
        location getsockname();

        void set_sockopt(int level, int optname,
                         const void *optval, socklen_t optlen);
        void set_reuseaddr();
        void set_tcp_nodelay();
        void sndbuf(size_t size);
        void rcvbuf(size_t size);
        void sndlowat(size_t size);
        void rcvlowat(size_t size);

        ssize_t recv(void *buf, size_t len, int flags);
        ssize_t xrecv(void *buf, size_t len, int flags);
        ssize_t send(const void *buf, size_t len, int flags);
        ssize_t xsend(const void *buf, size_t len, int flags);

    public:
        socket& operator = (int f);

    private:
        PO6_NONCOPYABLE(socket);
};

inline
socket :: socket()
    : fd(-1)
{
}

inline
socket :: socket(int domain, int type, int protocol)
    : fd(::socket(domain, type, protocol))
{
    if (get() < 0)
    {
        throw po6::error(errno);
    }
}

inline
socket :: ~socket() throw ()
{
}

// On all platforms I checked, a sockaddr_in6 is larger than a
// sockaddr, which in turn is larger than a sockaddr_in.  As a
// result, we allocate the largest of the three and call it a
// sockaddr.

inline void
socket :: bind(const ipaddr& addr, in_port_t port)
{
    sockaddr_in6 sa6;
    socklen_t salen = sizeof(sa6);
    sockaddr* sa = reinterpret_cast<sockaddr*>(&sa6);
    addr.pack(sa, &salen, port);

    if (::bind(get(), sa, salen) < 0)
    {
        throw po6::error(errno);
    }
}

inline void
socket :: bind(const ipaddr& addr)
{
    bind(addr, 0);
}

inline void
socket :: bind(const location& loc)
{
    bind(loc.address, loc.port);
}

inline void
socket :: connect(const ipaddr& addr, in_port_t port)
{
    sockaddr_in6 sa6;
    socklen_t salen = sizeof(sa6);
    sockaddr* sa = reinterpret_cast<sockaddr*>(&sa6);
    addr.pack(sa, &salen, port);

    if (::connect(get(), sa, salen) < 0)
    {
        throw po6::error(errno);
    }
}

inline void
socket :: connect(const location& loc)
{
    connect(loc.address, loc.port);
}

inline void
socket :: listen(int backlog)
{
    if (::listen(get(), backlog) < 0)
    {
        throw po6::error(errno);
    }
}

inline void
socket :: accept(socket* newsock)
{
    if (newsock->get() != -1)
    {
        throw std::logic_error("Accepting would overwrite preexisting socket.");
    }

    int ret;

    if ((ret = ::accept(get(), NULL, NULL)) < 0)
    {
        throw po6::error(errno);
    }

    *newsock = ret;
};

inline void
socket :: shutdown(int how)
{
    if (::shutdown(get(), how) < 0)
    {
        throw po6::error(errno);
    }
}

inline void
socket :: reset(int domain, int type, int protocol)
{
    socket s(domain, type, protocol);
    this->swap(&s);
}

inline location
socket :: getpeername()
{
    sockaddr_in6 sa6;
    socklen_t salen = sizeof(sa6);
    sockaddr* sa = reinterpret_cast<sockaddr*>(&sa6);

    if (::getpeername(get(), sa, &salen) < 0)
    {
        throw po6::error(errno);
    }

    return location(sa, salen);
}

inline location
socket :: getsockname()
{
    sockaddr_in6 sa6;
    socklen_t salen = sizeof(sa6);
    sockaddr* sa = reinterpret_cast<sockaddr*>(&sa6);

    if (::getsockname(get(), sa, &salen) < 0)
    {
        throw po6::error(errno);
    }

    return location(sa, salen);
}

inline void
socket :: set_sockopt(int level, int optname,
                      const void *optval, socklen_t optlen)
{
    if (setsockopt(get(), level, optname, optval, optlen) < 0)
    {
        throw po6::error(errno);
    }
}

inline void
socket :: set_reuseaddr()
{
    int yes = 1;
    set_sockopt(SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes));
}

inline void
socket :: set_tcp_nodelay()
{
    int yes = 1;
    set_sockopt(IPPROTO_TCP, TCP_NODELAY, &yes, sizeof(yes));
}

inline void
socket :: sndbuf(size_t size)
{
    set_sockopt(SOL_SOCKET, SO_SNDBUF, &size, sizeof(size));
}

inline void
socket :: rcvbuf(size_t size)
{
    set_sockopt(SOL_SOCKET, SO_RCVBUF, &size, sizeof(size));
}

inline void
socket :: sndlowat(size_t size)
{
    set_sockopt(SOL_SOCKET, SO_SNDLOWAT, &size, sizeof(size));
}

inline void
socket :: rcvlowat(size_t size)
{
    set_sockopt(SOL_SOCKET, SO_RCVLOWAT, &size, sizeof(size));
}

inline ssize_t
socket :: recv(void *buf, size_t len, int flags)
{
    return ::recv(get(), buf, len, flags);
}

inline ssize_t
socket :: xrecv(void *buf, size_t len, int flags)
{
    size_t rem = len;
    ssize_t amt = 0;

    while (rem > 0)
    {
        if ((amt = recv(buf, rem, flags)) < 0)
        {
            if (rem == len)
            {
                return -1;
            }
            else
            {
                break;
            }
        }
        else if (amt == 0)
        {
            break;
        }

        rem -= amt;
        buf = static_cast<char*>(buf) + amt;
    }

    return len - rem;
}

inline ssize_t
socket :: send(const void *buf, size_t len, int flags)
{
    return ::send(get(), buf, len, flags);
}

inline ssize_t
socket :: xsend(const void *buf, size_t len, int flags)
{
    size_t rem = len;
    ssize_t amt = 0;

    while (rem > 0)
    {
        if ((amt = send(buf, rem, flags)) < 0)
        {
            if (rem == len)
            {
                return -1;
            }
            else
            {
                break;
            }
        }
        else if (amt == 0)
        {
            break;
        }

        rem -= amt;
        buf = static_cast<const char*>(buf) + amt;
    }

    return len - rem;
}

inline socket&
socket :: operator = (int f)
{
    *dynamic_cast<fd*>(this) = f;
    return *this;
}

} // namespace net
} // namespace po6

#endif // po6_net_socket_h_
