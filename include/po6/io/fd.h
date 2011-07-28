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

#ifndef po6_io_fd_h_
#define po6_io_fd_h_

// POSIX
#include <errno.h>
#include <fcntl.h>

// po6
#include "po6/error.h"

#define PO6_IO_CLOSE_ACTION(e) \
    do \
    { \
        std::cerr << e.what() << std::endl; \
        abort(); \
    } while (0)

namespace po6
{
namespace io
{

class fd
{
    public:
        fd()
            : m_fd(-1)
        {
        }

        explicit fd(int f)
            : m_fd(f)
        {
        }

        virtual ~fd() throw ()
        {
            try
            {
                close();
            }
            catch (...)
            {
                try
                {
                    PO6_DTOR_ERROR("Unable to close file descriptor.");
                }
                catch (...)
                {
                }
            }
        }

    public:
        int get()
        {
            return m_fd;
        }

        void close()
        {
            if (m_fd >= 0)
            {
                if (::close(m_fd) < 0)
                {
                    throw po6::error(errno);
                }

                m_fd = -1;
            }
        }

        size_t read(void* buf, size_t size)
        {
            ssize_t ret = ::read(m_fd, buf, size);

            if (ret < 0)
            {
                throw po6::error(errno);
            }

            return ret;
        }

        size_t xread(void* buf, size_t size)
        {
            size_t rem;
            ssize_t amt;
            amt = 0;
            rem = size;

            while (rem > 0)
            {
                if ((amt = ::read(m_fd, buf, rem)) < 0)
                {
                    if (rem == size)
                    {
                        throw po6::error(errno);
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

            return size - rem;
        }

        size_t write(const void* buf, size_t size)
        {
            ssize_t ret = ::write(m_fd, buf, size);

            if (ret < 0)
            {
                throw po6::error(errno);
            }

            return ret;
        }

        size_t xwrite(const void* buf, size_t size)
        {
            size_t rem;
            ssize_t amt;
            amt = 0;
            rem = size;

            while (rem > 0)
            {
                if ((amt = ::write(m_fd, buf, rem)) < 0)
                {
                    if (rem == size)
                    {
                        throw po6::error(errno);
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

            return size - rem;
        }

        void nonblocking(bool enabled = true)
        {
            long flags = enabled ? O_NONBLOCK : 0;

            if (fcntl(get(), F_SETFL, flags) < 0)
            {
                throw po6::error(errno);
            }
        }

        void swap(fd* other) throw ()
        {
            int tmp = this->m_fd;
            this->m_fd = other->m_fd;
            other->m_fd = tmp;
        }

    public:
        fd& operator = (int f)
        {
            if (m_fd >= 0)
            {
                throw std::logic_error("Cannot assign to fd which represents an open file.");
            }

            m_fd = f;
            return *this;
        }

    private:
        fd(const fd&);

    private:
        fd& operator = (const fd&);

    private:
        int m_fd;
};

} // namespace io
} // namespace po6

#endif // po6_io_fd_h_
