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

#ifndef po6_error_h_
#define po6_error_h_

// C
#include <cstdlib>
#include <cstring>

// C++
#include <iostream>
#include <exception>
#include <stdexcept>

namespace po6
{

#define PO6_ERROR_MSG_LEN 1024

class error : public std::exception
{
    public:
        error();
        error(int num);
        error(const error& e);
        ~error() throw ();

    public:
        const char* what() const throw ();

    public:
        operator int () const;
        error& operator = (const error& rhs);

    private:
        int m_errno;
        mutable char m_msg[PO6_ERROR_MSG_LEN];
};

inline
error :: error()
    : m_errno(0)
{
}

inline
error :: error(int num)
    : m_errno(num)
{
}

inline
error :: error(const error& e)
    : m_errno(e.m_errno)
{
}

inline
error :: ~error() throw ()
{
}

inline const char*
error :: what() const throw ()
{
#ifdef _GNU_SOURCE
    if (_GNU_SOURCE)
    {
        return strerror_r(m_errno, m_msg, PO6_ERROR_MSG_LEN);
    }
#endif
    strerror_r(m_errno, m_msg, PO6_ERROR_MSG_LEN);
    return m_msg;
}

inline
error :: operator int () const
{
    return m_errno;
}

inline error&
error :: operator = (const error& rhs)
{
    m_errno = rhs.m_errno;
    return *this;
}

#undef PO6_ERROR_MSG_LEN

} // namespace po6

#endif // po6_error_h_
