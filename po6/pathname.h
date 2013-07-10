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

#ifndef po6_pathname_h_
#define po6_pathname_h_

// C
#include <cassert>

// POSIX
#include <errno.h>
#include <limits.h>

// C++
#include <iostream>

// STL
#include <string>

// po6
#include <po6/error.h>
#include <po6/io/fd.h>

namespace po6
{

class pathname
{
    public:
        pathname();
        pathname(const char* path);
        pathname(const std::string& path);
        pathname(const pathname& path);
        ~pathname() throw ();

    public:
        pathname basename() const;
        pathname dirname() const;
        pathname realpath() const;

    public:
        const char* get() const { return m_path; }

    public:
        bool operator == (const pathname& rhs) const;
        bool operator != (const pathname& rhs) const;

    private:
        friend std::ostream& operator << (std::ostream& lhs, const pathname& rhs);
        friend po6::pathname join(const pathname& a, const pathname& b);
        friend bool mkstemp(po6::io::fd* fd, po6::pathname* prefix);

    private:
        void initialize(const char* path);

    private:
        char m_path[PATH_MAX];
};

inline
pathname :: pathname()
    : m_path()
{
    initialize("");
}

inline
pathname :: pathname(const char* path)
    : m_path()
{
    initialize(path);
}

inline
pathname :: pathname(const std::string& path)
    : m_path()
{
    initialize(path.c_str());
}

inline
pathname :: pathname(const pathname& path)
    : m_path()
{
    initialize(path.m_path);
}

inline
pathname :: ~pathname() throw ()
{
}

inline pathname
pathname :: basename() const
{
    pathname base;

    // An empty path is just curdir
    if (this->m_path[0] == '\0')
    {
        strncpy(base.m_path, ".", PATH_MAX);
        return base;
    }

    size_t len = strlen(this->m_path);
    const char* start;
    const char* end = this->m_path + len - 1;

    // Remove trailing slashes
    while (end > this->m_path && *end == '/')
    {
        -- end;
    }

    // If we had all slashes
    if (end == this->m_path && this->m_path[0] == '/')
    {
        strncpy(base.m_path, "/", PATH_MAX);
    }

    // Work backwords to find the start
    start = end;

    while (start > this->m_path && *(start - 1) != '/')
    {
        -- start;
    }

    strncpy(base.m_path, start, (end - start + 1));
    base.m_path[end - start + 1] = '\0';
    return base;
}

inline pathname
pathname :: dirname() const
{
    pathname dir;

    // An empty path is just curdir
    if (this->m_path[0] == '\0')
    {
        strncpy(dir.m_path, ".", PATH_MAX);
        return dir;
    }

    size_t len = strlen(this->m_path);
    const char* end = this->m_path + len - 1;

    // Remove trailing slashes
    while (end > this->m_path && *end == '/')
    {
        -- end;
    }

    // Where do we divide the current directory and the last?
    while (end > this->m_path && *end != '/')
    {
        -- end;
    }

    // If we have reduced ourself to one character.
    if (end == this->m_path)
    {
        if (this->m_path[0] == '/')
        {
            strncpy(dir.m_path, "/", PATH_MAX);
        }
        else
        {
            strncpy(dir.m_path, ".", PATH_MAX);
        }

        return dir;
    }

    // Skip over the separator.
    do
    {
        -- end;
    }
    while (end > this->m_path && *end == '/');

    strncpy(dir.m_path, this->m_path, (end - this->m_path + 1));
    dir.m_path[end - this->m_path + 1] = '\0';
    return dir;
}

inline pathname
pathname :: realpath() const
{
    pathname tmp;
    memset(tmp.m_path, 0, sizeof(tmp.m_path));
    char* ret = ::realpath(m_path, tmp.m_path);

    if (ret == NULL)
    {
        throw po6::error(errno);
    }

    assert(ret == tmp.m_path);
    return tmp;
}

inline bool
pathname :: operator == (const pathname& rhs) const
{
    const pathname& lhs(*this);

    if (strncmp(lhs.m_path, rhs.m_path, PATH_MAX) == 0)
    {
        return true;
    }

    pathname lb = lhs.basename();
    pathname rb = rhs.basename();
    pathname ld = lhs.dirname();
    pathname rd = rhs.dirname();
    return strncmp(lb.m_path, rb.m_path, PATH_MAX) == 0 && ld == rd;
}

inline bool
pathname :: operator != (const pathname& rhs) const
{
    const pathname& lhs(*this);
    return !(lhs == rhs);
}

inline void
pathname :: initialize(const char* path)
{
    if (path)
    {
        strncpy(m_path, path, PATH_MAX);

        if (m_path[PATH_MAX - 1] != '\0')
        {
            throw po6::error(ENAMETOOLONG);
        }
    }
    else
    {
        strncpy(m_path, "", PATH_MAX);
    }
}

inline po6::pathname
join(const pathname& a, const pathname& b)
{
    if (b.m_path[0] == '/')
    {
        return b;
    }

    pathname ret(a);

    if (ret == "")
    {
        ret = ".";
    }

    size_t size_ret = strlen(ret.m_path);
    size_t size_b = strlen(b.m_path);

    if (size_ret + 1 + size_b >= PATH_MAX)
    {
        throw po6::error(ENAMETOOLONG);
    }

    char* end_of_ret = ret.m_path + size_ret - 1;

    // Remove trailing slashes
    while (end_of_ret > ret.m_path && *end_of_ret == '/')
    {
        -- end_of_ret;
    }

    ++ end_of_ret;
    *end_of_ret = '/';
    ++ end_of_ret;
    strncpy(end_of_ret, b.m_path, PATH_MAX - size_ret);
    return ret;
}

inline po6::pathname
join(const pathname& a, const pathname& b, const pathname& c)
{
    return join(join(a, b), c);
}

inline po6::pathname
join(const pathname& a, const pathname& b, const pathname& c, const pathname& d)
{
    return join(join(a, b, c), d);
}

inline std::ostream&
operator << (std::ostream& lhs, const pathname& rhs)
{
    lhs << rhs.m_path;
    return lhs;
}

// Unlike mkstemp, XXXXXX will be appended to prefix.  If there is not space to
// append XXXXXX to prefix, errno will be set to ENAMETOOLONG.  Unlike mkstemp,
// prefix will be unaltered in the case of an error.
inline bool
mkstemp(po6::io::fd* fd, po6::pathname* prefix)
{
    size_t len = strnlen(prefix->m_path, PATH_MAX);

    if (len + 6 >= PATH_MAX)
    {
        return ENAMETOOLONG;
    }

    po6::pathname tmp = *prefix;
    strncpy(prefix->m_path + len, "XXXXXX", PATH_MAX - len);
    *fd = ::mkstemp(prefix->m_path);

    if (fd->get() < 0)
    {
        *prefix = tmp;
        return false;
    }
    else
    {
        return true;
    }
}

} // namespace po6

#endif // po6_pathname_h_
