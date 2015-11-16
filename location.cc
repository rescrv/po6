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
#include "po6/net/location.h"

using po6::net::location;

uint64_t
location :: hash(const location& loc)
{
    uint64_t ip_hash = ipaddr::hash(loc.address);
    return ip_hash ^ loc.port;
}

location :: location()
    : address()
    , port()
{
}

location :: location(const ipaddr& _address)
    : address(_address)
    , port()
{
}

location :: location(const ipaddr& _address, in_port_t _port)
    : address(_address)
    , port(_port)
{
}

location :: location(const sockaddr_in* sa)
    : address()
    , port()
{
    set(sa);
}

location :: location(const sockaddr_in6* sa)
    : address()
    , port()
{
    set(sa);
}

location :: location(const location& other)
    : address(other.address)
    , port(other.port)
{
}

location :: ~location() throw ()
{
}

void
location :: pack(struct sockaddr* addr, socklen_t* addrlen) const
{
    address.pack(addr, addrlen, port);
}

bool
location :: set(const char* addr)
{
    return address.set(addr);
}

bool
location :: set(const char* addr, in_port_t p)
{
    if (address.set(addr))
    {
        port = p;
        return true;
    }

    return false;
}

bool
location :: set(const sockaddr* sa, socklen_t salen)
{
    if (sa->sa_family == AF_INET && salen >= sizeof(sockaddr_in))
    {
        set(reinterpret_cast<const sockaddr_in*>(sa));
        return true;
    }
    else if (sa->sa_family == AF_INET6 && salen >= sizeof(sockaddr_in6))
    {
        set(reinterpret_cast<const sockaddr_in6*>(sa));
        return true;
    }
    else
    {
        return false;
    }
}

void
location :: set(const sockaddr_in* sa)
{
    address = ipaddr(sa->sin_addr);
    port = ntohs(sa->sin_port);
}

void
location :: set(const sockaddr_in6* sa)
{
    address = ipaddr(sa->sin6_addr);
    port = ntohs(sa->sin6_port);
}

location&
location :: operator = (const location& rhs)
{
    if (this != &rhs)
    {
        address = rhs.address;
        port = rhs.port;
    }

    return *this;
}

bool
location :: operator < (const location& rhs) const
{
    return compare(rhs) < 0;
}

bool
location :: operator <= (const location& rhs) const
{
    return compare(rhs) <= 0;
}

bool
location :: operator == (const location& rhs) const
{
    return compare(rhs) == 0;
}

bool
location :: operator != (const location& rhs) const
{
    return compare(rhs) != 0;
}

bool
location :: operator >= (const location& rhs) const
{
    return compare(rhs) >= 0;
}

bool
location :: operator > (const location& rhs) const
{
    return compare(rhs) > 0;
}

int
location :: compare(const location& rhs) const
{
    const location& lhs(*this);

    if (lhs.address < rhs.address)
    {
        return -1;
    }
    else if (lhs.address > rhs.address)
    {
        return 1;
    }

    return lhs.port - rhs.port;
}

std::ostream&
po6 :: net :: operator << (std::ostream& lhs, const location& rhs)
{
    if (rhs.address.family() == AF_INET6)
    {
        lhs << "[" << rhs.address << "]:" << rhs.port;
    }
    else
    {
        lhs << rhs.address << ":" << rhs.port;
    }

    return lhs;
}
