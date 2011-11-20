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

// Google Test
#include <gtest/gtest.h>

// po6
#include "../include/po6/pathname.h"

#pragma GCC diagnostic ignored "-Wswitch-default"

namespace
{

// Can we create paths using common representations of paths.
TEST(PathnameTest, CtorAndDtor)
{
    po6::pathname p1;
    po6::pathname p2(NULL);
    po6::pathname p3("/foo");
    po6::pathname p4(std::string("/foo"));
    po6::pathname p5(p1);
}

TEST(PathnameTest, Equality)
{
    {
        po6::pathname p1;
        po6::pathname p2("");
        EXPECT_EQ(p1, p2);
    }

    {
        po6::pathname p1("/foo");
        po6::pathname p2("/foo");
        EXPECT_EQ(p1, p2);
    }

    {
        po6::pathname p1("/foo");
        po6::pathname p2("/foo/");
        EXPECT_EQ(p1, p2);
    }

    {
        po6::pathname p1("/foo");
        po6::pathname p2("/foo/.");
        EXPECT_NE(p1, p2);
    }

    {
        po6::pathname p1("//foo");
        po6::pathname p2("/foo");
        EXPECT_EQ(p1, p2);
    }

    {
        po6::pathname p1("./foo");
        po6::pathname p2("foo");
        EXPECT_EQ(p1, p2);
    }

// I haven't made up my mind as to whether this should be "equal".
#if 0
    {
        po6::pathname p1("foo/./bar");
        po6::pathname p2("foo/bar");
        EXPECT_EQ(p1, p2);
    }
#endif

    {
        po6::pathname p1("foo/../bar");
        po6::pathname p2("foo/bar");
        EXPECT_NE(p1, p2);
    }
}

TEST(PathnameTest, Basename)
{
    EXPECT_EQ(po6::pathname().basename(), po6::pathname("."));
    EXPECT_EQ(po6::pathname(NULL).basename(), po6::pathname("."));
    EXPECT_EQ(po6::pathname("").basename(), po6::pathname("."));
    EXPECT_EQ(po6::pathname(".").basename(), po6::pathname("."));
    EXPECT_EQ(po6::pathname("..").basename(), po6::pathname(".."));
    EXPECT_EQ(po6::pathname("/").basename(), po6::pathname("/"));
    EXPECT_EQ(po6::pathname("////////").basename(), po6::pathname("/"));
    EXPECT_EQ(po6::pathname("bar").basename(), po6::pathname("bar"));
    EXPECT_EQ(po6::pathname("/foo/bar").basename(), po6::pathname("bar"));
    EXPECT_EQ(po6::pathname("/foo/bar/").basename(), po6::pathname("bar"));
    EXPECT_EQ(po6::pathname("/foo/bar/.").basename(), po6::pathname("."));
    EXPECT_EQ(po6::pathname("foo/bar").basename(), po6::pathname("bar"));
    EXPECT_EQ(po6::pathname("foo/bar/").basename(), po6::pathname("bar"));
    EXPECT_EQ(po6::pathname("foo/bar/.").basename(), po6::pathname("."));
}

TEST(PathnameTest, Dirname)
{
    EXPECT_EQ(po6::pathname().dirname(), po6::pathname("."));
    EXPECT_EQ(po6::pathname(NULL).dirname(), po6::pathname("."));
    EXPECT_EQ(po6::pathname("").dirname(), po6::pathname("."));
    EXPECT_EQ(po6::pathname(".").dirname(), po6::pathname("."));
    EXPECT_EQ(po6::pathname("..").dirname(), po6::pathname("."));
    EXPECT_EQ(po6::pathname("/").dirname(), po6::pathname("/"));
    EXPECT_EQ(po6::pathname("////////").dirname(), po6::pathname("/"));
    EXPECT_EQ(po6::pathname("bar").dirname(), po6::pathname("."));
    EXPECT_EQ(po6::pathname("/foo/bar").dirname(), po6::pathname("/foo"));
    EXPECT_EQ(po6::pathname("/foo/bar/").dirname(), po6::pathname("/foo"));
    EXPECT_EQ(po6::pathname("/foo/bar/.").dirname(), po6::pathname("/foo/bar"));
    EXPECT_EQ(po6::pathname("foo/bar").dirname(), po6::pathname("foo"));
    EXPECT_EQ(po6::pathname("foo/bar/").dirname(), po6::pathname("foo"));
    EXPECT_EQ(po6::pathname("foo/bar/.").dirname(), po6::pathname("foo/bar"));
}

TEST(PathnameTest, Join)
{
    EXPECT_EQ(po6::pathname("foo/bar"), po6::join(po6::pathname("foo"), po6::pathname("bar")));
    EXPECT_EQ(po6::pathname("/bar"), po6::join(po6::pathname("foo"), po6::pathname("/bar")));
    EXPECT_EQ(po6::pathname("./bar"), po6::join(po6::pathname(""), po6::pathname("bar")));
    EXPECT_EQ(po6::pathname("foo/bar"), po6::join(po6::pathname("foo/"), po6::pathname("bar")));
    EXPECT_EQ(po6::pathname("foo/bar/baz"), po6::join(po6::pathname("foo"), po6::pathname("bar"), po6::pathname("baz")));
    EXPECT_EQ(po6::pathname("foo/bar/baz/quux"), po6::join(po6::pathname("foo"), po6::pathname("bar"), po6::pathname("baz"), po6::pathname("quux")));
}

} // namespace
