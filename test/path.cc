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

// POSIX
#include <errno.h>

// po6
#include "th.h"
#include "po6/path.h"

namespace
{

TEST(PathnameTest, Basename)
{
    ASSERT_EQ(po6::path::basename(""), ".");
    ASSERT_EQ(po6::path::basename("."), ".");
    ASSERT_EQ(po6::path::basename(".."), "..");
    ASSERT_EQ(po6::path::basename("/"), "/");
    ASSERT_EQ(po6::path::basename("////////"), "/");
    ASSERT_EQ(po6::path::basename("bar"), "bar");
    ASSERT_EQ(po6::path::basename("/foo/bar"), "bar");
    ASSERT_EQ(po6::path::basename("/foo/bar/"), "bar");
    ASSERT_EQ(po6::path::basename("/foo/bar/."), ".");
    ASSERT_EQ(po6::path::basename("foo/bar"), "bar");
    ASSERT_EQ(po6::path::basename("foo/bar/"), "bar");
    ASSERT_EQ(po6::path::basename("foo/bar/."), ".");
}

TEST(PathnameTest, Dirname)
{
    ASSERT_EQ(po6::path::dirname(""), ".");
    ASSERT_EQ(po6::path::dirname("."), ".");
    ASSERT_EQ(po6::path::dirname(".."), ".");
    ASSERT_EQ(po6::path::dirname("/"), "/");
    ASSERT_EQ(po6::path::dirname("////////"), "/");
    ASSERT_EQ(po6::path::dirname("bar"), ".");
    ASSERT_EQ(po6::path::dirname("/foo/bar"), "/foo");
    ASSERT_EQ(po6::path::dirname("/foo/bar/"), "/foo");
    ASSERT_EQ(po6::path::dirname("/foo/bar/."), "/foo/bar");
    ASSERT_EQ(po6::path::dirname("foo/bar"), "foo");
    ASSERT_EQ(po6::path::dirname("foo/bar/"), "foo");
    ASSERT_EQ(po6::path::dirname("foo/bar/."), "foo/bar");
}

TEST(PathnameTest, Join)
{
    ASSERT_EQ("foo/bar", po6::path::join("foo", "bar"));
    ASSERT_EQ("/bar", po6::path::join("foo", "/bar"));
    ASSERT_EQ("./bar", po6::path::join("", "bar"));
    ASSERT_EQ("foo/bar", po6::path::join("foo/", "bar"));
    ASSERT_EQ("foo/bar/baz", po6::path::join("foo", "bar", "baz"));
    ASSERT_EQ("foo/bar/baz/quux", po6::path::join("foo", "bar", "baz", "quux"));
    ASSERT_EQ("/quux", po6::path::join("foo", "bar", "baz", "/quux"));
    ASSERT_EQ("/bar", po6::path::join("///", "/bar"));
}

} // namespace
