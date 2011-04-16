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
        location()
            : address(), port()
        {
        }

        location(const ipaddr& _address, in_port_t _port)
            : address(_address), port(_port)
        {
        }

        ~location()
        {
        }

    public:
        bool operator < (const location& rhs) const
        {
            const location& lhs(*this);

            if (lhs.address == rhs.address)
            {
                return lhs.port < rhs.port;
            }
            else
            {
                return lhs.address < rhs.address;
            }
        }

        bool operator == (const location& rhs) const
        {
            const location& lhs(*this);
            return (lhs.address == rhs.address) && (lhs.port == rhs.port);
        }

        bool operator != (const location& rhs) const
        {
            const location& lhs(*this);
            return !(lhs == rhs);
        }

    public:
        ipaddr address;
        in_port_t port;
};

inline std::ostream&
operator << (std::ostream& lhs, const location& rhs)
{
    lhs << rhs.address << ":" << rhs.port;
    return lhs;
}

} // namespace net
} // namespace po6

#endif // po6_net_location_h_
