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

#ifndef po6_net_location_h_
#define po6_net_location_h_

// C++
#include <iostream>

// po6
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
        explicit location(const ipaddr& address);
        location(const ipaddr& address, in_port_t port);
        explicit location(const sockaddr_in* sa);
        explicit location(const sockaddr_in6* sa);
        location(const location& other);
        ~location() throw ();

    public:
        void pack(struct sockaddr* addr, socklen_t* addrlen) const;
        PO6_WARN_UNUSED bool set(const char* address);
        PO6_WARN_UNUSED bool set(const char* address, in_port_t port);
        PO6_WARN_UNUSED bool set(const sockaddr* sa, socklen_t salen);
        void set(const sockaddr_in* sa);
        void set(const sockaddr_in6* sa);

    public:
        location& operator = (const location& rhs);
        bool operator < (const location& rhs) const;
        bool operator <= (const location& rhs) const;
        bool operator == (const location& rhs) const;
        bool operator != (const location& rhs) const;
        bool operator >= (const location& rhs) const;
        bool operator > (const location& rhs) const;

    public:
        ipaddr address;
        in_port_t port;

    private:
        int compare(const location& rhs) const;
};

std::ostream&
operator << (std::ostream& lhs, const location& rhs);

} // namespace net
} // namespace po6

#endif // po6_net_location_h_
