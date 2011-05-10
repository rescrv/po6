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

// C++
#include <sstream>

// Google Test
#include <gtest/gtest.h>

// po6
#include "po6/net/ipaddr.h"

#pragma GCC diagnostic ignored "-Wswitch-default"

namespace
{

TEST(IpaddrTest, CreateAndCompare)
{
    in_addr ipv4;
    in6_addr ipv6;

    ASSERT_EQ(inet_pton(AF_INET, "127.0.0.1", &ipv4), 1);
    ASSERT_EQ(inet_pton(AF_INET6, "::1", &ipv6), 1);

    po6::net::ipaddr a;
    po6::net::ipaddr b("127.0.0.1");
    po6::net::ipaddr c("::1");
    po6::net::ipaddr d(ipv4);
    po6::net::ipaddr e(ipv6);
    po6::net::ipaddr aprime(a);
    po6::net::ipaddr bprime(b);
    po6::net::ipaddr cprime(c);
    po6::net::ipaddr dprime(d);
    po6::net::ipaddr eprime(e);

    EXPECT_EQ(a, aprime);
    EXPECT_EQ(b, bprime);
    EXPECT_EQ(c, cprime);
    EXPECT_EQ(d, dprime);
    EXPECT_EQ(e, eprime);

    EXPECT_NE(a, b);
    EXPECT_NE(b, c);
    EXPECT_NE(c, d);
    EXPECT_NE(d, e);
}

TEST(IpaddrTest, Assignment)
{
    po6::net::ipaddr lhs;
    po6::net::ipaddr rhs("127.0.0.1");

    EXPECT_NE(lhs, rhs);
    lhs = rhs;
    EXPECT_EQ(lhs, rhs);
}

TEST(IpaddrTest, SelfAssignment)
{
    po6::net::ipaddr ip("127.0.0.1");

    EXPECT_EQ(ip, po6::net::ipaddr("127.0.0.1"));
    EXPECT_EQ(ip = ip, po6::net::ipaddr("127.0.0.1"));
    EXPECT_EQ(ip, po6::net::ipaddr("127.0.0.1"));
}

TEST(IpaddrTest, PackV4)
{
    sockaddr sa;
    socklen_t sz = sizeof(sa);
    po6::net::ipaddr ip("127.0.0.1");

    EXPECT_EQ(ip.family(), AF_INET);
    ip.pack(&sa, &sz, 1234);
    EXPECT_EQ(reinterpret_cast<sockaddr_in*>(&sa)->sin_family, AF_INET);
    EXPECT_EQ(reinterpret_cast<sockaddr_in*>(&sa)->sin_addr.s_addr, htonl(0x7f000001));
    EXPECT_EQ(reinterpret_cast<sockaddr_in*>(&sa)->sin_port, htons(1234));
}

TEST(IpaddrTest, PackV6)
{
    sockaddr_in6 sa;
    socklen_t sz = sizeof(sa);
    po6::net::ipaddr ip("::1");
    in6_addr ipv6;

    ASSERT_EQ(inet_pton(AF_INET6, "::1", &ipv6), 1);

    EXPECT_EQ(ip.family(), AF_INET6);
    ip.pack(reinterpret_cast<sockaddr*>(&sa), &sz, 1234);
    EXPECT_EQ(sa.sin6_family, AF_INET6);
    EXPECT_EQ(sa.sin6_port, htons(1234));
    EXPECT_EQ(memcmp(&sa.sin6_addr, &ipv6, sizeof(in6_addr)), 0);
}

TEST(IpaddrTest, OutputStream)
{
    po6::net::ipaddr a("127.0.0.1");
    po6::net::ipaddr b("0:0:0:0:0:0:0:0");
    po6::net::ipaddr c("1:0:0:0:0:0:0:8");
    po6::net::ipaddr d("0:0:0:0:0:FFFF:204.152.189.116");

    std::ostringstream sa;
    std::ostringstream sb;
    std::ostringstream sc;
    std::ostringstream sd;

    sa << a;
    sb << b;
    sc << c;
    sd << d;

    EXPECT_EQ("127.0.0.1", sa.str());
    EXPECT_EQ("::", sb.str());
    EXPECT_EQ("1::8", sc.str());
    EXPECT_EQ("::ffff:204.152.189.116", sd.str());
}

} // namespace
