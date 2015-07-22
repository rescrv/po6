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
#include <limits.h>
#include <stdlib.h>
#include <string.h>

// po6
#include "po6/path.h"

std::string
po6 :: path :: basename(const std::string& _path)
{
    const char* path = _path.c_str();
    std::string base;

    // An empty path is just curdir
    if (path[0] == '\0')
    {
        return ".";
    }

    size_t len = strlen(path);
    const char* start;
    const char* end = path + len - 1;

    // Remove trailing slashes
    while (end > path && *end == '/')
    {
        --end;
    }

    // If we had all slashes
    if (end == path && path[0] == '/')
    {
        base = "/";
    }

    // Work backwords to find the start
    start = end;

    while (start > path && *(start - 1) != '/')
    {
        --start;
    }

    base.assign(start, end - start + 1);
    return base;
}

std::string
po6 :: path :: dirname(const std::string& _path)
{
    const char* path = _path.c_str();
    std::string dir;

    // An empty path is just curdir
    if (path[0] == '\0')
    {
        return ".";
    }

    size_t len = strlen(path);
    const char* end = path + len - 1;

    // Remove trailing slashes
    while (end > path && *end == '/')
    {
        --end;
    }

    // Where do we divide the current directory and the last?
    while (end > path && *end != '/')
    {
        --end;
    }

    // If we have reduced ourself to one character.
    if (end == path)
    {
        if (path[0] == '/')
        {
            return "/";
        }
        else
        {
            return ".";
        }
    }

    // Skip over the separator.
    do
    {
        --end;
    }
    while (end > path && *end == '/');

    dir.assign(path, (end - path) + 1);
    return dir;
}

bool
po6 :: path :: realpath(const std::string& path, std::string* real)
{
    char buf[PATH_MAX + 1];
    memset(buf, 0, sizeof(buf));
    char* ret = ::realpath(path.c_str(), buf);

    if (ret == NULL)
    {
        return false;
    }

    buf[PATH_MAX] = '\0';
    *real = buf;
    return true;
}

std::string
po6 :: path :: join(const std::string& p1,
                    const std::string& p2)
{
    if (!p2.empty() && p2[0] == '/')
    {
        return p2;
    }

    std::string ret = p1;

    if (ret == "")
    {
        ret = ".";
    }

    while (!ret.empty() && ret[ret.size() - 1] == '/')
    {
        ret.erase(ret.size() - 1);
    }

    return ret + "/" + p2;
}

std::string
po6 :: path :: join(const std::string& p1,
                    const std::string& p2,
                    const std::string& p3)
{
    return join(join(p1, p2), p3);
}

std::string
po6 :: path :: join(const std::string& p1,
                    const std::string& p2,
                    const std::string& p3,
                    const std::string& p4)
{
    return join(join(p1, p2, p3), p4);
}

std::string
po6 :: path :: join(const std::string& p1,
                    const std::string& p2,
                    const std::string& p3,
                    const std::string& p4,
                    const std::string& p5)
{
    return join(join(p1, p2, p3, p4), p5);
}
