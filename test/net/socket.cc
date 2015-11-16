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

// po6
#include "th.h"
#include "po6/net/ipaddr.h"
#include "po6/net/location.h"
#include "po6/net/socket.h"

po6::net::ipaddr
IPADDR(const char* address)
{
    po6::net::ipaddr ip;
    bool ret = ip.set(address);
    ASSERT_TRUE(ret);
    return ip;
}

po6::net::location
LOCATION(const char* address, in_port_t port)
{
    po6::net::location loc;
    bool ret = loc.set(address, port);
    ASSERT_TRUE(ret);
    return loc;
}

namespace
{

TEST(SocketTest, ClientAndServer)
{
    po6::net::socket server;
    po6::net::socket client;
    ASSERT_TRUE(server.reset(AF_INET, SOCK_STREAM, IPPROTO_TCP));
    ASSERT_TRUE(client.reset(AF_INET, SOCK_STREAM, IPPROTO_TCP));

    // Create the sockaddr.
    sockaddr sa;
    socklen_t salen = sizeof(sa);
    po6::net::ipaddr ip = IPADDR("127.0.0.1");
    ip.pack(&sa, &salen, INADDR_ANY);

    // Create the server.
    server.bind(ip);
    server.listen(10);

    // Create the client.
    po6::net::location loc;
    ASSERT_TRUE(server.getsockname(&loc));
    client.connect(loc);

    // Close down the connection.
    client.shutdown(SHUT_RDWR);
    server.shutdown(SHUT_RDWR);
    client.close();
    server.close();
}

} // namespace
