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
#include "po6/io/fd.h"

using po6::io::fd;

fd :: fd()
    : m_fd(-1)
{
}

fd :: fd(int f)
    : m_fd(f)
{
}

fd :: ~fd() throw ()
{
    close();
}

void
fd :: close()
{
    if (m_fd >= 0)
    {
        ::close(m_fd);
    }

    m_fd = -1;
}

ssize_t
fd :: read(void *buf, size_t nbytes)
{
    return ::read(m_fd, buf, nbytes);
}

ssize_t
fd :: xread(void* buf, size_t nbytes)
{
    size_t rem = nbytes;
    ssize_t amt = 0;

    while (rem > 0)
    {
        if ((amt = read(buf, rem)) < 0)
        {
            if (rem == nbytes)
            {
                return -1;
            }
            else
            {
                break;
            }
        }
        else if (amt == 0)
        {
            break;
        }

        rem -= amt;
        buf = static_cast<char*>(buf) + amt;
    }

    return nbytes - rem;
}

ssize_t
fd :: write(const void *buf, size_t nbytes)
{
    return ::write(m_fd, buf, nbytes);
}

ssize_t
fd :: xwrite(const void *buf, size_t nbytes)
{
    size_t rem = nbytes;
    ssize_t amt = 0;

    while (rem > 0)
    {
        if ((amt = write(buf, rem)) < 0)
        {
            if (rem == nbytes)
            {
                return -1;
            }
            else
            {
                break;
            }
        }
        else if (amt == 0)
        {
            break;
        }

        rem -= amt;
        buf = static_cast<const char*>(buf) + amt;
    }

    return nbytes - rem;
}

bool
fd :: set_nonblocking()
{
    long flags = O_NONBLOCK;
    return fcntl(get(), F_SETFL, flags) >= 0;
}

void
fd :: swap(fd* other) throw ()
{
    int tmp = this->m_fd;
    this->m_fd = other->m_fd;
    other->m_fd = tmp;
}

fd&
fd :: operator = (int f)
{
    close();
    m_fd = f;
    return *this;
}
