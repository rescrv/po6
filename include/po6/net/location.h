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

#ifndef po6_net_location_h_
#define po6_net_location_h_

// C++
#include <iostream>

// po6
#include <po6/error.h>
#include <po6/net/ipaddr.h>

namespace po6
{
namespace net
{

class location
{
    public:
        static uint64_t hash(const location& loc);

    public:
        location();
        location(const ipaddr& _address, in_port_t _port = 0);
        location(const char* _address, in_port_t _port = 0);
        location(const sockaddr* sa, socklen_t salen);
        explicit location(const sockaddr_in* sa);
        explicit location(const sockaddr_in6* sa);
        ~location() throw ();

    public:
        void pack(struct sockaddr* addr, socklen_t* addrlen) const;
        void set(const sockaddr* sa, socklen_t salen);
        void set(const sockaddr_in* sa);
        void set(const sockaddr_in6* sa);

    public:
        bool operator < (const location& rhs) const { return compare(rhs) < 0; }
        bool operator <= (const location& rhs) const { return compare(rhs) <= 0; }
        bool operator == (const location& rhs) const { return compare(rhs) == 0; }
        bool operator != (const location& rhs) const { return compare(rhs) != 0; }
        bool operator >= (const location& rhs) const { return compare(rhs) >= 0; }
        bool operator > (const location& rhs) const { return compare(rhs) > 0; }

    public:
        ipaddr address;
        in_port_t port;

    private:
        int compare(const location& rhs) const
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
};

inline uint64_t
location :: hash(const location& loc)
{
    uint64_t ip_hash = ipaddr::hash(loc.address);
    return ip_hash ^ loc.port;
}

inline
location :: location()
    : address()
    , port()
{
}

inline
location :: location(const ipaddr& _address, in_port_t _port)
    : address(_address)
    , port(_port)
{
}

inline
location :: location(const char* _address, in_port_t _port)
    : address(_address)
    , port(_port)
{
}

inline
location :: location(const sockaddr* sa, socklen_t salen)
    : address()
    , port()
{
    set(sa, salen);
}

inline
location :: location(const sockaddr_in* sa)
    : address()
    , port()
{
    set(sa);
}

inline
location :: location(const sockaddr_in6* sa)
    : address()
    , port()
{
    set(sa);
}

inline
location :: ~location() throw ()
{
}

inline void
location :: pack(struct sockaddr* addr, socklen_t* addrlen) const
{
    address.pack(addr, addrlen, port);
}

inline void
location :: set(const sockaddr* sa, socklen_t salen)
{
    if (sa->sa_family == AF_INET && salen >= sizeof(sockaddr_in))
    {
        set(reinterpret_cast<const sockaddr_in*>(sa));
    }
    else if (sa->sa_family == AF_INET6 && salen >= sizeof(sockaddr_in6))
    {
        set(reinterpret_cast<const sockaddr_in6*>(sa));
    }
    else
    {
        throw std::runtime_error("Socket address family unknown (or sockaddr too small).");
    }
}

inline void
location :: set(const sockaddr_in* sa)
{
    address = ipaddr(sa->sin_addr);
    port = ntohs(sa->sin_port);
}

inline void
location :: set(const sockaddr_in6* sa)
{
    address = ipaddr(sa->sin6_addr);
    port = ntohs(sa->sin6_port);
}

inline std::ostream&
operator << (std::ostream& lhs, const location& rhs)
{
    lhs << rhs.address << ":" << rhs.port;
    return lhs;
}

} // namespace net
} // namespace po6

#endif // po6_net_location_h_
