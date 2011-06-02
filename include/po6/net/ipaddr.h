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

#ifndef po6_net_ipaddr_h_
#define po6_net_ipaddr_h_

// POSIX
#include <arpa/inet.h>
#include <errno.h>
#include <sys/socket.h>

// STL
#include <string>

// po6
#include <po6/error.h>

namespace po6
{
namespace net
{

class ipaddr
{
    public:
        static ipaddr ANY() { return ipaddr(INADDR_ANY); }

    public:
        ipaddr()
            : m_family(AF_UNSPEC)
            , m_ip()
        {
        }

        ipaddr(const char* addr)
            : m_family(AF_UNSPEC)
            , m_ip()
        {
            set(addr);
        }

        ipaddr(const std::string& addr)
            : m_family(AF_UNSPEC)
            , m_ip()
        {
            set(addr);
        }

        ipaddr(const in_addr& ipv4)
            : m_family(AF_UNSPEC)
            , m_ip()
        {
            set(ipv4);
        }
        ipaddr(const in_addr_t& ipv4)
            : m_family(AF_UNSPEC)
            , m_ip()
        {
            in_addr ia;
            ia.s_addr = ipv4;
            set(ia);
        }

        ipaddr(const in6_addr& ipv6)
            : m_family(AF_UNSPEC)
            , m_ip()
        {
            set(ipv6);
        }

        ipaddr(const ipaddr& other)
            : m_family(other.m_family)
            , m_ip(other.m_ip)
        {
        }

        ~ipaddr() throw ()
        {
        }

    public:
        int family() const
        {
            return m_family;
        }

        void pack(struct sockaddr* addr, socklen_t* addrlen, in_port_t port) const
        {
            memset(addr, 0, *addrlen);

            if (m_family == AF_UNSPEC)
            {
                throw std::logic_error("cannot pack a sockaddr with AF_UNSPEC");
            }
            else if (m_family == AF_INET && *addrlen >= sizeof(sockaddr_in))
            {
                pack(reinterpret_cast<sockaddr_in*>(addr), port);
                *addrlen = sizeof(sockaddr_in);
            }
            else if (m_family == AF_INET6 && *addrlen >= sizeof(sockaddr_in6))
            {
                pack(reinterpret_cast<sockaddr_in6*>(addr), port);
                *addrlen = sizeof(sockaddr_in6);
            }
            else
            {
                throw std::length_error("insufficiently sized sockaddr");
            }
        }

        void pack(struct sockaddr_in* addr, in_port_t port) const
        {
            if (m_family != AF_INET)
            {
                throw std::logic_error("Cannot pack non-AF_INET sockaddr into sockaddr_in.");
            }

            addr->sin_family = AF_INET;
            addr->sin_port = htons(port);
            memmove(&addr->sin_addr, &m_ip.v4, sizeof(in_addr));
        }

        void pack(struct sockaddr_in6* addr, in_port_t port) const
        {
            if (m_family != AF_INET6)
            {
                throw std::logic_error("Cannot pack non-AF_INET6 sockaddr into sockaddr_in6.");
            }

            addr->sin6_family = AF_INET6;
            addr->sin6_port = htons(port);
            memmove(&addr->sin6_addr, &m_ip.v6, sizeof(in6_addr));
        }

        int compare(const ipaddr& rhs) const
        {
            const ipaddr& lhs(*this);

            if (lhs.m_family != rhs.m_family)
            {
                return lhs.m_family - rhs.m_family;
            }

            if (lhs.m_family == AF_INET)
            {
                return memcmp(&lhs.m_ip.v4, &rhs.m_ip.v4, sizeof(in_addr));
            }
            else if (lhs.m_family == AF_INET6)
            {
                return memcmp(&lhs.m_ip.v6, &rhs.m_ip.v6, sizeof(in6_addr));
            }
            else
            {
                return 0;
            }
        }

    public:
        void set(const char* addr)
        {
            in_addr ipv4;
            in6_addr ipv6;

            if (inet_pton(AF_INET, addr, &ipv4) > 0)
            {
                set(ipv4);
            }
            else if (inet_pton(AF_INET6, addr, &ipv6) > 0)
            {
                set(ipv6);
            }
            else
            {
                if (errno != 0 && errno != EAFNOSUPPORT)
                {
                    throw po6::error(errno);
                }
                else
                {
                    throw std::invalid_argument("malformed address");
                }
            }
        }

        void set(const std::string& s)
        {
            set(s.c_str());
        }

        void set(const in_addr& ipv4)
        {
            m_family = AF_INET;
            memmove(&m_ip.v4, &ipv4, sizeof(in_addr));
        }

        void set(const in6_addr& ipv6)
        {
            m_family = AF_INET6;
            memmove(&m_ip.v6, &ipv6, sizeof(in6_addr));
        }

    public:
        ipaddr& operator = (const ipaddr& rhs)
        {
            if (this != &rhs)
            {
                m_family = rhs.m_family;
                memmove(&m_ip, &rhs.m_ip, sizeof(m_ip));
            }

            return *this;
        }

        bool operator < (const ipaddr& rhs) const
        {
            return compare(rhs) < 0;
        }

        bool operator == (const ipaddr& rhs) const
        {
            return compare(rhs) == 0;
        }

        bool operator != (const ipaddr& rhs) const
        {
            return compare(rhs) != 0;
        }

    private:
        friend std::ostream& operator << (std::ostream& lhs, const ipaddr& rhs);

    private:
        int m_family;
        union
        {
            in_addr v4;
            in6_addr v6;
        } m_ip;
};

inline std::ostream&
operator << (std::ostream& lhs, const ipaddr& rhs)
{
    if (rhs.m_family == AF_UNSPEC)
    {
        lhs << "UNSPECIFIED";
    }
    else if (rhs.m_family == AF_INET)
    {
        char repr[INET_ADDRSTRLEN];

        if (!inet_ntop(AF_INET, &rhs.m_ip.v4, repr, INET_ADDRSTRLEN))
        {
            throw std::logic_error("inet_ntop failed, but should never fail.");
        }

        lhs << std::string(repr);
    }
    else if (rhs.m_family == AF_INET6)
    {
        char repr[INET6_ADDRSTRLEN];

        if (!inet_ntop(AF_INET6, &rhs.m_ip.v6, repr, INET6_ADDRSTRLEN))
        {
            throw std::logic_error("inet_ntop failed, but should never fail.");
        }

        lhs << std::string(repr);
    }

    return lhs;
}

inline std::istream&
operator >> (std::istream& lhs, ipaddr& rhs)
{
    try
    {
        std::string s;
        lhs >> s;
        rhs.set(s);
    }
    catch (po6::error& e)
    {
        lhs.setstate(std::istream::failbit);
    }
    catch (std::invalid_argument& e)
    {
        lhs.setstate(std::istream::failbit);
    }

    return lhs;
}

} // namespace net
} // namespace po6

#endif // po6_net_ipaddr_h_
