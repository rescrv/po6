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

// C++
#include <tr1/functional>

// po6
#include "th.h"
#include "po6/threads/spinlock.h"
#include "po6/threads/thread.h"

class SpinlockTestThread
{
    public:
        SpinlockTestThread(po6::threads::spinlock* sp)
            : m_sp(sp)
        {
        }

        ~SpinlockTestThread()
        {
        }

        void operator () ()
        {
            for (int i = 0; i < 1000000; ++i)
            {
                m_sp->lock();
                m_sp->unlock();
            }
        }

    private:
        SpinlockTestThread(const SpinlockTestThread&);

    private:
        SpinlockTestThread& operator = (const SpinlockTestThread&);

    private:
        po6::threads::spinlock* m_sp;
};

namespace
{

TEST(SpinlockTest, CtorAndDtor)
{
    po6::threads::spinlock sp;
}

TEST(SpinlockTest, LockAndUnlock)
{
    po6::threads::spinlock sp;
    sp.lock();
    sp.unlock();
}

TEST(SpinlockTest, TryLock)
{
    po6::threads::spinlock sp;
    ASSERT_TRUE(sp.trylock());
    ASSERT_FALSE(sp.trylock());
    sp.unlock();
}

TEST(SpinlockTest, TwoThreads)
{
    po6::threads::spinlock sp;
    SpinlockTestThread mtt(&sp);
    po6::threads::thread t1(std::tr1::ref(mtt));
    po6::threads::thread t2(std::tr1::ref(mtt));

    t1.start();
    t2.start();
    t1.join();
    t2.join();
}

TEST(SpinlockTest, Holding)
{
    po6::threads::spinlock sp;

    {
        po6::threads::spinlock::hold h(&sp);
        ASSERT_FALSE(sp.trylock());
    }

    ASSERT_TRUE(sp.trylock());
    sp.unlock();
}

} // namespace
