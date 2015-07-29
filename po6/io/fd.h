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

#ifndef po6_io_fd_h_
#define po6_io_fd_h_

// POSIX
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>

// po6
#include <po6/errno.h>

namespace po6
{
namespace io
{

class fd
{
    public:
        fd();
        explicit fd(int f);
        virtual ~fd() throw ();

    public:
        int get() const { return m_fd; }
        void close();
        PO6_WARN_UNUSED ssize_t read(void *buf, size_t nbytes);
        PO6_WARN_UNUSED ssize_t xread(void* buf, size_t nbytes);
        PO6_WARN_UNUSED ssize_t write(const void *buf, size_t nbytes);
        PO6_WARN_UNUSED ssize_t xwrite(const void *buf, size_t nbytes);
        PO6_WARN_UNUSED bool set_nonblocking();
        void swap(fd* other) throw ();

    public:
        fd& operator = (int f);

    private:
        int m_fd;

    private:
        fd(const fd&);
        fd& operator = (const fd&);
};

} // namespace io
} // namespace po6

#endif // po6_io_fd_h_
