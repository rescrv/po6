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

// C
#include <stdlib.h>
#include <string.h>

// STL
#include <iostream>

// po6
#include "po6/net/ipaddr.h"

using po6::net::ipaddr;

ipaddr :: ipaddr()
    : m_family(AF_UNSPEC)
    , m_ip()
{
}

ipaddr :: ipaddr(const in_addr& ipv4)
    : m_family(AF_UNSPEC)
    , m_ip()
{
    set(ipv4);
}

ipaddr :: ipaddr(const in_addr_t& ipv4)
    : m_family(AF_UNSPEC)
    , m_ip()
{
    in_addr ia;
    ia.s_addr = ipv4;
    set(ia);
}

ipaddr :: ipaddr(const in6_addr& ipv6)
    : m_family(AF_UNSPEC)
    , m_ip()
{
    set(ipv6);
}

ipaddr :: ipaddr(const ipaddr& other)
    : m_family(other.m_family)
    , m_ip(other.m_ip)
{
}

ipaddr :: ~ipaddr() throw ()
{
}

uint64_t
ipaddr :: hash(const ipaddr& ip)
{
    uint64_t ret = 0;

    if (ip.m_family == AF_INET)
    {
        ret = ip.m_ip.v4.s_addr;
    }
    else if (ip.m_family == AF_INET6)
    {
        const uint8_t* addr = &ip.m_ip.v6.s6_addr[0];
        uint64_t low = 0;
        uint64_t high = 0;
        memmove(&low, addr, sizeof(uint64_t));
        memmove(&high, addr + 8, sizeof(uint64_t));
        return low ^ high;
    }

    return ret;
}

void
ipaddr :: pack(struct sockaddr* addr, socklen_t* addrlen, in_port_t port) const
{
    memset(addr, 0, *addrlen);

    if (m_family == AF_INET && *addrlen >= sizeof(sockaddr_in))
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
        abort();
    }
}

void
ipaddr :: pack(struct sockaddr_in* addr, in_port_t port) const
{
    assert(m_family == AF_INET);
    addr->sin_family = AF_INET;
    addr->sin_port = htons(port);
    memmove(&addr->sin_addr, &m_ip.v4, sizeof(in_addr));
}

void
ipaddr :: pack(struct sockaddr_in6* addr, in_port_t port) const
{
    assert(m_family == AF_INET6);
    addr->sin6_family = AF_INET6;
    addr->sin6_port = htons(port);
    memmove(&addr->sin6_addr, &m_ip.v6, sizeof(in6_addr));
}

int
ipaddr :: compare(const ipaddr& rhs) const
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

bool
ipaddr :: set(const char* addr)
{
    in_addr ipv4;
    in6_addr ipv6;

    if (inet_pton(AF_INET, addr, &ipv4) > 0)
    {
        set(ipv4);
        return true;
    }
    else if (inet_pton(AF_INET6, addr, &ipv6) > 0)
    {
        set(ipv6);
        return true;
    }
    else
    {
        return false;
    }
}

bool
ipaddr :: set(const std::string& s)
{
    return set(s.c_str());
}

void
ipaddr :: set(const in_addr& ipv4)
{
    m_family = AF_INET;
    memmove(&m_ip.v4, &ipv4, sizeof(in_addr));
}

void
ipaddr :: set(const in6_addr& ipv6)
{
    m_family = AF_INET6;
    memmove(&m_ip.v6, &ipv6, sizeof(in6_addr));
}

ipaddr&
ipaddr :: operator = (const ipaddr& rhs)
{
    if (this != &rhs)
    {
        m_family = rhs.m_family;
        memmove(&m_ip, &rhs.m_ip, sizeof(m_ip));
    }

    return *this;
}

bool
ipaddr :: operator < (const ipaddr& rhs) const
{
    return compare(rhs) < 0;
}

bool
ipaddr :: operator <= (const ipaddr& rhs) const
{
    return compare(rhs) <= 0;
}

bool
ipaddr :: operator == (const ipaddr& rhs) const
{
    return compare(rhs) == 0;
}

bool
ipaddr :: operator != (const ipaddr& rhs) const
{
    return compare(rhs) != 0;
}

bool
ipaddr :: operator >= (const ipaddr& rhs) const
{
    return compare(rhs) >= 0;
}

bool
ipaddr :: operator > (const ipaddr& rhs) const
{
    return compare(rhs) > 0;
}

std::ostream&
po6 :: net :: operator << (std::ostream& lhs, const ipaddr& rhs)
{
    if (rhs.m_family == AF_UNSPEC)
    {
        lhs << "UNSPECIFIED";
    }
    else if (rhs.m_family == AF_INET)
    {
        char repr[INET_ADDRSTRLEN];
        const char* s = inet_ntop(AF_INET, &rhs.m_ip.v4, repr, INET_ADDRSTRLEN);
        assert(s);
        lhs << std::string(s);
    }
    else if (rhs.m_family == AF_INET6)
    {
        char repr[INET6_ADDRSTRLEN];
        const char* s = inet_ntop(AF_INET6, &rhs.m_ip.v6, repr, INET6_ADDRSTRLEN);
        assert(s);
        lhs << std::string(s);
    }

    return lhs;
}

std::istream&
po6 :: net :: operator >> (std::istream& lhs, ipaddr& rhs)
{
    std::string s;
    lhs >> s;

    if (!rhs.set(s))
    {
        lhs.setstate(std::istream::failbit);
    }

    return lhs;
}
