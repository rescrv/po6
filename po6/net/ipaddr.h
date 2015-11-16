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

#ifndef po6_net_ipaddr_h_
#define po6_net_ipaddr_h_

// C
#include <assert.h>

// POSIX
#include <arpa/inet.h>
#include <errno.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>

// STL
#include <string>

// po6
#include <po6/errno.h>

namespace po6
{
namespace net
{

class ipaddr
{
    public:
        static ipaddr ANY() { return ipaddr(INADDR_ANY); }
        static uint64_t hash(const ipaddr& ip);

    public:
        ipaddr();
        explicit ipaddr(const in_addr& ipv4);
        explicit ipaddr(const in_addr_t& ipv4);
        explicit ipaddr(const in6_addr& ipv6);
        ipaddr(const ipaddr& other);
        ~ipaddr() throw ();

    public:
        int family() const { return m_family; }
        void pack(struct sockaddr* addr, socklen_t* addrlen, in_port_t port) const;
        void pack(struct sockaddr_in* addr, in_port_t port) const;
        void pack(struct sockaddr_in6* addr, in_port_t port) const;
        const in_addr& v4addr() const { assert(m_family == AF_INET); return m_ip.v4; }
        const in6_addr& v6addr() const { assert(m_family == AF_INET6); return m_ip.v6; }
        int compare(const ipaddr& rhs) const;

    public:
        PO6_WARN_UNUSED bool set(const char* addr);
        PO6_WARN_UNUSED bool set(const std::string& s);
        void set(const in_addr& ipv4);
        void set(const in6_addr& ipv6);

    public:
        ipaddr& operator = (const ipaddr& rhs);
        bool operator < (const ipaddr& rhs) const;
        bool operator <= (const ipaddr& rhs) const;
        bool operator == (const ipaddr& rhs) const;
        bool operator != (const ipaddr& rhs) const;
        bool operator >= (const ipaddr& rhs) const;
        bool operator > (const ipaddr& rhs) const;

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

std::ostream&
operator << (std::ostream& lhs, const ipaddr& rhs);

std::istream&
operator >> (std::istream& lhs, ipaddr& rhs);

} // namespace net
} // namespace po6

#endif // po6_net_ipaddr_h_
