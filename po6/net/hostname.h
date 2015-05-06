// Copyright (c) 2012, Robert Escriva
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

#ifndef po6_net_hostname_h_
#define po6_net_hostname_h_

// C
#include <stdio.h>

// POSIX
#ifndef _MSC_VER
#include <netdb.h>
#endif

// STL
#include <sstream>

// po6
#include <po6/net/location.h>
#include <po6/net/socket.h>

namespace po6
{
namespace net
{

class hostname
{
    public:
        hostname();
        explicit hostname(const char* _address, in_port_t _port);
        explicit hostname(const po6::net::location&);
        hostname(const hostname& other);
        ~hostname() throw ();

    public:
        location connect(int domain, int type, int protocol, po6::net::socket* sock) const;
#ifndef _MSC_VER
        // non-throwing, non-connecting version
        location lookup(int type, int protocol) const;
#endif

    public:
        bool operator < (const hostname& rhs) const { return compare(rhs) < 0; }
        bool operator <= (const hostname& rhs) const { return compare(rhs) <= 0; }
        bool operator == (const hostname& rhs) const { return compare(rhs) == 0; }
        bool operator != (const hostname& rhs) const { return compare(rhs) != 0; }
        bool operator >= (const hostname& rhs) const { return compare(rhs) >= 0; }
        bool operator > (const hostname& rhs) const { return compare(rhs) > 0; }

    public:
        std::string address;
        in_port_t port;

    private:
        class protect_addrinfo;

    private:
        int compare(const hostname& rhs) const;
};

class hostname::protect_addrinfo
{
    public:
        protect_addrinfo(addrinfo* ai) : m_ai(ai) {}
        ~protect_addrinfo() throw () { if (m_ai) { freeaddrinfo(m_ai); } }

    private:
        addrinfo* m_ai;
        protect_addrinfo(const protect_addrinfo&);
        protect_addrinfo& operator = (const protect_addrinfo&);
};

inline
hostname :: hostname()
    : address()
    , port()
{
}

inline
hostname :: hostname(const char* _hostname, in_port_t _port)
    : address(_hostname)
    , port(_port)
{
}

inline
hostname :: hostname(const po6::net::location& loc)
    : address()
    , port(loc.port)
{
    std::ostringstream ostr;
    ostr << loc.address;
    address = ostr.str();
}

inline
hostname :: hostname(const hostname& other)
    : address(other.address)
    , port(other.port)
{
}

inline
hostname :: ~hostname() throw ()
{
}

inline po6::net::location
hostname :: connect(int domain, int type, int protocol, po6::net::socket* sock) const
{
    // Convert the port to a C string
    char port_cstr[6];
#ifdef _MSC_VER
	sprintf_s(port_cstr,"%u", port);
#else
    snprintf(port_cstr, 6, "%u", port);
#endif

    // Setup the hints
    addrinfo hints;
    memset(&hints, 0, sizeof(hints));
    hints.ai_flags = AI_ADDRCONFIG | AI_NUMERICSERV;
    hints.ai_family = domain;
    hints.ai_socktype = type;
    hints.ai_protocol = protocol;

    // Make the call
    addrinfo* res = NULL;
    int gai_err = getaddrinfo(address.c_str(), port_cstr, &hints, &res);

    if (gai_err)
    {
        throw po6::error(gai_err, gai_strerror(gai_err));
    }

    protect_addrinfo pai(res);
    location loc;

    for (addrinfo* ai = res; ai; ai = ai->ai_next)
    {
        if (ai->ai_family != AF_INET && ai->ai_family != AF_INET6)
        {
            errno = ECONNREFUSED;
            continue;
        }

        sock->reset(ai->ai_family, ai->ai_socktype, ai->ai_protocol);

        if (sock->get() < 0)
        {
            continue;
        }

        if (::connect(sock->get(), ai->ai_addr, ai->ai_addrlen) < 0)
        {
            sock->close();
            continue;
        }

        loc = location(ai->ai_addr, ai->ai_addrlen);
        break;
    }

    if (sock->get() < 0)
    {
        throw po6::error(errno);
    }

    return loc;
}

#ifndef _MSC_VER
inline po6::net::location
hostname :: lookup(int type, int protocol) const
{
    // Convert the port to a C string
    char port_cstr[6];
    snprintf(port_cstr, 6, "%u", port);

    // Setup the hints
    addrinfo hints;
    memset(&hints, 0, sizeof(hints));
    hints.ai_flags = AI_ADDRCONFIG | AI_NUMERICSERV;
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = type;
    hints.ai_protocol = protocol;

    // Make the call
    addrinfo* res = NULL;
    int gai_err = getaddrinfo(address.c_str(), port_cstr, &hints, &res);

    if (gai_err)
    {
        return po6::net::location();
    }

    protect_addrinfo pai(res);
    location loc;

    for (addrinfo* ai = res; ai; ai = ai->ai_next)
    {
        if (ai->ai_family != AF_INET && ai->ai_family != AF_INET6)
        {
            errno = ECONNREFUSED;
            continue;
        }

        loc = location(ai->ai_addr, ai->ai_addrlen);
        break;
    }

    return loc;
}
#endif

inline int
hostname :: compare(const hostname& rhs) const
{
    if (address < rhs.address)
    {
        return -1;
    }

    if (address > rhs.address)
    {
        return 1;
    }

    if (port < rhs.port)
    {
        return -1;
    }

    if (port > rhs.port)
    {
        return -1;
    }

    return 0;
}

inline std::ostream&
operator << (std::ostream& lhs, const hostname& rhs)
{
    if (rhs.address.find(':') != std::string::npos)
    {
        lhs << "[" << rhs.address << "]:" << rhs.port;
    }
    else
    {
        lhs << rhs.address << ":" << rhs.port;
    }

    return lhs;
}

} // namespace net
} // namespace po6

#endif // po6_net_hostname_h_
