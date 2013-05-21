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
#include <fcntl.h>

// C++
#include <iostream>

// po6
#include "th.h"
#include "po6/io/fd.h"

namespace
{

TEST(FdTest, CtorAndDtor)
{
    po6::io::fd fd;
}

TEST(FdTest, OpenAndExplicitClose)
{
    po6::io::fd fd;
    fd = open("/dev/zero", O_RDONLY);
    fd.close();
}

TEST(FdTest, OpenAndImplicitClose)
{
    po6::io::fd fd;
    fd = open("/dev/zero", O_RDONLY);
}

TEST(FdTest, OpenReadWriteClose)
{
    po6::io::fd fd;
    fd = open("/dev/zero", O_RDWR);
    char buf[4096];
    fd.read(buf, 4096);

    for (int i = 0; i < 4096; ++i)
    {
        ASSERT_EQ(buf[i], '\0');
    }

    fd.write(buf, 4096);
    fd.close();
}

} // namespace
