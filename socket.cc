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

// po6
#include "po6/net/socket.h"

po6 :: net :: socket :: socket()
    : fd(-1)
{
}

po6 :: net :: socket :: ~socket() throw ()
{
}

bool
po6 :: net :: socket :: reset(int domain, int type, int protocol)
{
    *this = ::socket(domain, type, protocol);
    return get() >= 0;
}

// On all platforms I checked, a sockaddr_in6 is larger than a
// sockaddr, which in turn is larger than a sockaddr_in.  As a
// result, we allocate the largest of the three and call it a
// sockaddr.

bool
po6 :: net :: socket :: bind(const ipaddr& addr, in_port_t port)
{
    sockaddr_in6 sa6;
    socklen_t salen = sizeof(sa6);
    sockaddr* sa = reinterpret_cast<sockaddr*>(&sa6);
    addr.pack(sa, &salen, port);
    return ::bind(get(), sa, salen) == 0;
}

bool
po6 :: net :: socket :: bind(const ipaddr& addr)
{
    return bind(addr, 0);
}

bool
po6 :: net :: socket :: bind(const location& loc)
{
    return bind(loc.address, loc.port);
}

bool
po6 :: net :: socket :: connect(const ipaddr& addr, in_port_t port)
{
    sockaddr_in6 sa6;
    socklen_t salen = sizeof(sa6);
    sockaddr* sa = reinterpret_cast<sockaddr*>(&sa6);
    addr.pack(sa, &salen, port);
    return ::connect(get(), sa, salen) == 0;
}

bool
po6 :: net :: socket :: connect(const location& loc)
{
    return connect(loc.address, loc.port);
}

bool
po6 :: net :: socket :: listen(int backlog)
{
    return ::listen(get(), backlog) == 0;
}

bool
po6 :: net :: socket :: accept(socket* newsock)
{
    newsock->close();
    int ret;

    if ((ret = ::accept(get(), NULL, NULL)) < 0)
    {
        return false;
    }

    *newsock = ret;
    return true;
}

bool
po6 :: net :: socket :: shutdown(int how)
{
    return ::shutdown(get(), how) == 0;
}

bool
po6 :: net :: socket :: getpeername(location* loc)
{
    sockaddr_in6 sa6;
    socklen_t salen = sizeof(sa6);
    sockaddr* sa = reinterpret_cast<sockaddr*>(&sa6);

    if (::getpeername(get(), sa, &salen) < 0)
    {
        return false;
    }

    return loc->set(sa, salen);
}

bool
po6 :: net :: socket :: getsockname(location* loc)
{
    sockaddr_in6 sa6;
    socklen_t salen = sizeof(sa6);
    sockaddr* sa = reinterpret_cast<sockaddr*>(&sa6);

    if (::getsockname(get(), sa, &salen) < 0)
    {
        return false;
    }

    return loc->set(sa, salen);
}

bool
po6 :: net :: socket :: set_sockopt(int level, int optname,
                                    const void *optval, socklen_t optlen)
{
    return setsockopt(get(), level, optname, optval, optlen) == 0;
}

bool
po6 :: net :: socket :: set_reuseaddr()
{
    int yes = 1;
    return set_sockopt(SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes));
}

bool
po6 :: net :: socket :: set_tcp_nodelay()
{
    int yes = 1;
    return set_sockopt(IPPROTO_TCP, TCP_NODELAY, &yes, sizeof(yes));
}

bool
po6 :: net :: socket :: sndbuf(size_t size)
{
    return set_sockopt(SOL_SOCKET, SO_SNDBUF, &size, sizeof(size));
}

bool
po6 :: net :: socket :: rcvbuf(size_t size)
{
    return set_sockopt(SOL_SOCKET, SO_RCVBUF, &size, sizeof(size));
}

bool
po6 :: net :: socket :: sndlowat(size_t size)
{
    return set_sockopt(SOL_SOCKET, SO_SNDLOWAT, &size, sizeof(size));
}

bool
po6 :: net :: socket :: rcvlowat(size_t size)
{
    return set_sockopt(SOL_SOCKET, SO_RCVLOWAT, &size, sizeof(size));
}

ssize_t
po6 :: net :: socket :: recv(void *buf, size_t len, int flags)
{
    return ::recv(get(), buf, len, flags);
}

ssize_t
po6 :: net :: socket :: xrecv(void *buf, size_t len, int flags)
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

ssize_t
po6 :: net :: socket :: send(const void *buf, size_t len, int flags)
{
    return ::send(get(), buf, len, flags);
}

ssize_t
po6 :: net :: socket :: xsend(const void *buf, size_t len, int flags)
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

po6::net::socket&
po6 :: net :: socket :: operator = (int f)
{
    *dynamic_cast<fd*>(this) = f;
    return *this;
}
