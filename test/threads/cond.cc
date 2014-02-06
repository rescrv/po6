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
#include "po6/threads/cond.h"
#include "po6/threads/mutex.h"
#include "po6/threads/thread.h"

#define ITERS 1000000

class CondTestSimpleCond
{
    public:
        CondTestSimpleCond(unsigned* counter, po6::threads::cond* cnd)
            : m_counter(counter)
            , m_cnd(cnd)
        {
        }

        CondTestSimpleCond(const CondTestSimpleCond& other)
            : m_counter(other.m_counter)
            , m_cnd(other.m_cnd)
        {
        }

    public:
        void operator () ()
        {
            m_cnd->lock();

            while (*m_counter < ITERS)
            {
                m_cnd->wait();
            }

            m_cnd->unlock();
        }

    private:
        CondTestSimpleCond& operator = (const CondTestSimpleCond&);

    private:
        unsigned* m_counter;
        po6::threads::cond* m_cnd;
};

namespace
{

TEST(CondTest, CtorAndDtor)
{
    po6::threads::mutex mtx;
    po6::threads::cond cnd(&mtx);
}

TEST(CondTest, SimpleCond)
{
    po6::threads::mutex mtx;
    po6::threads::cond cnd(&mtx);
    unsigned counter;
    CondTestSimpleCond ctsc(&counter, &cnd);
    po6::threads::thread t(ctsc);
    t.start();

    for (unsigned i = 0; i <= ITERS; ++i)
    {
        cnd.lock();
        counter = i;

        if (i % 2 == 0)
        {
            cnd.signal();
        }
        else
        {
            cnd.broadcast();
        }

        cnd.unlock();
    }

    t.join();
}

} // namespace
