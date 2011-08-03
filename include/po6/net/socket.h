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

namespace po6
{
namespace net
{

class socket : public po6::io::fd
{
    public:
        // The only use for sockets from this constructor are to assign to with accept.
        socket()
            : fd(-1)
        {
        }

        socket(int domain, int type, int protocol)
            : fd(::socket(domain, type, protocol))
        {
            if (get() < 0)
            {
                throw po6::error(errno);
            }
        }

        ~socket() throw ()
        {
        }

    public:
        // TODO: bind, connect
        // On all platforms I checked, a sockaddr_in6 is larger than a
        // sockaddr, which in turn is larger than a sockaddr_in.  As a
        // result, we allocate the largest of the three and call it a
        // sockaddr.

        void bind(const ipaddr& addr, in_port_t port)
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

        void bind(const ipaddr& addr)
        {
            // TODO:  I'm sure there is a named constant for this.
            bind(addr, 0);
        }

        void bind(const location& loc)
        {
            bind(loc.address, loc.port);
        }

        void connect(const ipaddr& addr, in_port_t port)
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

        void connect(const location& loc)
        {
            connect(loc.address, loc.port);
        }

        void listen(int backlog)
        {
            if (::listen(get(), backlog) < 0)
            {
                throw po6::error(errno);
            }
        }

        void accept(socket* newsock)
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

        void shutdown(int how)
        {
            if (::shutdown(get(), how) < 0)
            {
                throw po6::error(errno);
            }
        }

        void reset(int domain, int type, int protocol)
        {
            socket s(domain, type, protocol);
            this->swap(&s);
        }

        location getpeername()
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

        location getsockname()
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

        void reuseaddr(bool setting)
        {
            int yes = setting ? 1 : 0;

            if (setsockopt(get(), SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes)) < 0)
            {
                throw po6::error(errno);
            }
        }

        void tcp_nodelay(bool setting)
        {
            int yes = setting ? 1 : 0;

            if (setsockopt(get(), IPPROTO_TCP, TCP_NODELAY, &yes, sizeof(yes)) < 0)
            {
                throw po6::error(errno);
            }
        }

        void sndbuf(size_t size)
        {
            if (setsockopt(get(), SOL_SOCKET, SO_SNDBUF, &size, sizeof(size)) < 0)
            {
                throw po6::error(errno);
            }
        }

        void rcvbuf(size_t size)
        {
            if (setsockopt(get(), SOL_SOCKET, SO_RCVBUF, &size, sizeof(size)) < 0)
            {
                throw po6::error(errno);
            }
        }

        void sndlowat(size_t size)
        {
            if (setsockopt(get(), SOL_SOCKET, SO_SNDLOWAT, &size, sizeof(size)) < 0)
            {
                throw po6::error(errno);
            }
        }

        void rcvlowat(size_t size)
        {
            if (setsockopt(get(), SOL_SOCKET, SO_RCVLOWAT, &size, sizeof(size)) < 0)
            {
                throw po6::error(errno);
            }
        }

        size_t recv(void* buf, size_t size, int flags)
        {
            ssize_t ret = ::recv(get(), buf, size, flags);

            if (ret < 0)
            {
                throw po6::error(errno);
            }

            return ret;
        }

        size_t send(const void* buf, size_t size, int flags)
        {
            ssize_t ret = ::send(get(), buf, size, flags);

            if (ret < 0)
            {
                throw po6::error(errno);
            }

            return ret;
        }

        size_t xsend(const void* buf, size_t size, int flags)
        {
            size_t rem;
            ssize_t amt;
            amt = 0;
            rem = size;

            while (rem > 0)
            {
                if ((amt = ::send(get(), buf, rem, flags)) < 0)
                {
                    if (rem == size)
                    {
                        throw po6::error(errno);
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

            return size - rem;
        }

    public:
        socket& operator = (int f)
        {
            if (get() >= 0)
            {
                throw std::logic_error("Cannot assign to sock which represents an open socket.");
            }

            *dynamic_cast<fd*>(this) = f;
            return *this;
        }

    private:
        socket(const socket&);

    private:
        socket& operator = (const socket&);
};

} // namespace net
} // namespace po6

#endif // po6_net_socket_h_
