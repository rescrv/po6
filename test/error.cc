// Copyright (c) 2011, Robert Escriva
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
//     * Redistributions of source code must retain the above copyright notice,
//     this list of conditions and the following disclaimer.
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
#include "error.h"

namespace
{

#pragma GCC diagnostic ignored "-Wswitch-default"
// logic_error should normally throw a std::logic_error
TEST(LogicErrorTest, NormallyThrows)
{
    bool caught = false;
    std::string msg = "message describing the logic error";

    try
    {
        po6::logic_error(msg);
    }
    catch (std::logic_error e)
    {
        EXPECT_EQ(msg, e.what());
        caught = true;
    }

    ASSERT_TRUE(caught);
}

class LogicErrorTestThrowInProgressDeathTest
{
    public:
        LogicErrorTestThrowInProgressDeathTest() {}
        ~LogicErrorTestThrowInProgressDeathTest() { po6::logic_error("msg"); }
};

// If there is a throw in-progress, logic_error must abort
TEST(LogicErrorTest, ThrowInProgressDeathTest)
{
    try
    {
        ASSERT_DEATH(
            LogicErrorTestThrowInProgressDeathTest l;
            throw std::exception();
        , "");
    }
    catch (...)
    {
    }
}

// Test the copying and assigning of errors
TEST(ErrorTest, CopyingAndAssigning)
{
    po6::error e0;
    po6::error e1(EBUSY);
    po6::error e2(e1);

    ASSERT_EQ(e0, 0);
    e0 = e2;
    ASSERT_EQ(e0, e1);
}

} // namespace
