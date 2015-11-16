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
#include <string.h>

// C++
#include <sstream>

// po6
#include "th.h"
#include "po6/net/ipaddr.h"

po6::net::ipaddr
IPADDR(const char* address)
{
    po6::net::ipaddr ip;
    bool ret = ip.set(address);
    ASSERT_TRUE(ret);
    return ip;
}

namespace
{

TEST(IpaddrTest, CreateAndCompare)
{
    in_addr ipv4;
    in6_addr ipv6;

    ASSERT_EQ(inet_pton(AF_INET, "127.0.0.1", &ipv4), 1);
    ASSERT_EQ(inet_pton(AF_INET6, "::1", &ipv6), 1);

    po6::net::ipaddr a;
    po6::net::ipaddr b = IPADDR("127.0.0.1");
    po6::net::ipaddr c = IPADDR("::1");
    po6::net::ipaddr d(ipv4);
    po6::net::ipaddr e(ipv6);
    po6::net::ipaddr aprime(a);
    po6::net::ipaddr bprime(b);
    po6::net::ipaddr cprime(c);
    po6::net::ipaddr dprime(d);
    po6::net::ipaddr eprime(e);

    ASSERT_EQ(a, aprime);
    ASSERT_EQ(b, bprime);
    ASSERT_EQ(c, cprime);
    ASSERT_EQ(d, dprime);
    ASSERT_EQ(e, eprime);

    ASSERT_NE(a, b);
    ASSERT_NE(b, c);
    ASSERT_NE(c, d);
    ASSERT_NE(d, e);
}

TEST(IpaddrTest, Assignment)
{
    po6::net::ipaddr lhs;
    po6::net::ipaddr rhs = IPADDR("127.0.0.1");

    ASSERT_NE(lhs, rhs);
    lhs = rhs;
    ASSERT_EQ(lhs, rhs);
}

TEST(IpaddrTest, SelfAssignment)
{
    po6::net::ipaddr ip = IPADDR("127.0.0.1");

    ASSERT_EQ(ip, IPADDR("127.0.0.1"));
    ASSERT_EQ(ip = ip, IPADDR("127.0.0.1"));
    ASSERT_EQ(ip, IPADDR("127.0.0.1"));
}

TEST(IpaddrTest, PackV4)
{
    sockaddr sa;
    socklen_t sz = sizeof(sa);
    po6::net::ipaddr ip = IPADDR("127.0.0.1");

    ASSERT_EQ(ip.family(), AF_INET);
    ip.pack(&sa, &sz, 1234);
    ASSERT_EQ(reinterpret_cast<sockaddr_in*>(&sa)->sin_family, AF_INET);
    ASSERT_EQ(reinterpret_cast<sockaddr_in*>(&sa)->sin_addr.s_addr, htonl(0x7f000001));
    ASSERT_EQ(reinterpret_cast<sockaddr_in*>(&sa)->sin_port, htons(1234));
}

TEST(IpaddrTest, PackV6)
{
    sockaddr_in6 sa;
    socklen_t sz = sizeof(sa);
    po6::net::ipaddr ip = IPADDR("::1");
    in6_addr ipv6;

    ASSERT_EQ(inet_pton(AF_INET6, "::1", &ipv6), 1);

    ASSERT_EQ(ip.family(), AF_INET6);
    ip.pack(reinterpret_cast<sockaddr*>(&sa), &sz, 1234);
    ASSERT_EQ(sa.sin6_family, AF_INET6);
    ASSERT_EQ(sa.sin6_port, htons(1234));
    ASSERT_EQ(memcmp(&sa.sin6_addr, &ipv6, sizeof(in6_addr)), 0);
}

TEST(IpaddrTest, OutputStream)
{
    po6::net::ipaddr a = IPADDR("127.0.0.1");
    po6::net::ipaddr b = IPADDR("0:0:0:0:0:0:0:0");
    po6::net::ipaddr c = IPADDR("1:0:0:0:0:0:0:8");
    po6::net::ipaddr d = IPADDR("0:0:0:0:0:FFFF:204.152.189.116");

    std::ostringstream sa;
    std::ostringstream sb;
    std::ostringstream sc;
    std::ostringstream sd;

    sa << a;
    sb << b;
    sc << c;
    sd << d;

    ASSERT_EQ("127.0.0.1", sa.str());
    ASSERT_EQ("::", sb.str());
    ASSERT_EQ("1::8", sc.str());
    ASSERT_EQ("::ffff:204.152.189.116", sd.str());
}

TEST(IpaddrTest, InputStream)
{
    std::istringstream sa("127.0.0.1");
    std::istringstream sb("0:0:0:0:0:0:0:0");
    std::istringstream sc("1:0:0:0:0:0:0:8");
    std::istringstream sd("0:0:0:0:0:FFFF:204.152.189.116");

    po6::net::ipaddr a;
    po6::net::ipaddr b;
    po6::net::ipaddr c;
    po6::net::ipaddr d;

    sa >> a;
    sb >> b;
    sc >> c;
    sd >> d;

    ASSERT_EQ(IPADDR("127.0.0.1"), a);
    ASSERT_EQ(IPADDR("::"), b);
    ASSERT_EQ(IPADDR("1::8"), c);
    ASSERT_EQ(IPADDR("::ffff:204.152.189.116"), d);
}

TEST(IpaddrTest, InputStreamErrors)
{
    std::istringstream sa("127:0.0.1");
    std::istringstream sb("0:0::0:0::0:0");
    std::istringstream sc("dead beef");

    po6::net::ipaddr a;
    po6::net::ipaddr b;
    po6::net::ipaddr c;

    sa >> a;
    sb >> b;
    sc >> c;

    ASSERT_FALSE(sa.good());
    ASSERT_FALSE(sb.good());
    ASSERT_FALSE(sc.good());
}

} // namespace
