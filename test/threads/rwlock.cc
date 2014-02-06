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

// po6
#include "th.h"
#include "po6/threads/rwlock.h"
#include "po6/threads/thread.h"

class RwlockTestThread
{
    public:
        RwlockTestThread(po6::threads::rwlock* rwl, bool mode)
            : m_rwl(rwl)
            , m_mode(mode)
        {
        }

        RwlockTestThread(const RwlockTestThread& other)
            : m_rwl(other.m_rwl)
            , m_mode(other.m_mode)
        {
        }

        void operator () ()
        {
            for (int i = 0; i < 1000000; ++i)
            {
                if (m_mode)
                {
                    m_rwl->rdlock();
                }
                else
                {
                    m_rwl->wrlock();
                }

                m_rwl->unlock();
            }
        }

    private:
        RwlockTestThread& operator = (const RwlockTestThread&);

    private:
        po6::threads::rwlock* m_rwl;
        bool m_mode;
};

namespace
{

TEST(RwlockTest, CtorAndDtor)
{
    po6::threads::rwlock rwl;
}

TEST(RwlockTest, LockAndUnlock)
{
    po6::threads::rwlock rwl;
    rwl.rdlock();
    rwl.unlock();
    rwl.wrlock();
    rwl.unlock();
}

TEST(RwlockTest, TwoThreads)
{
    po6::threads::rwlock rwl;
    RwlockTestThread read(&rwl, true);
    RwlockTestThread write(&rwl, false);
    po6::threads::thread t1(read);
    po6::threads::thread t2(read);
    po6::threads::thread t3(write);

    t1.start();
    t2.start();
    t3.start();
    t1.join();
    t2.join();
    t3.join();
}

TEST(RwlockTest, Holding)
{
    po6::threads::rwlock rwl;

    {
        po6::threads::rwlock::rdhold h(&rwl);
    }

    {
        po6::threads::rwlock::wrhold h(&rwl);
    }
}

} // namespace
