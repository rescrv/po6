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

#ifndef po6_threads_cond_h
#define po6_threads_cond_h

// POSIX
#include <errno.h>
#include <pthread.h>

// po6
#include <po6/noncopyable.h>
#include <po6/threads/mutex.h>

namespace po6
{
namespace threads
{

class cond
{
    public:
        cond(mutex* mtx);
        ~cond() throw ();

    public:
        void lock();
        void unlock();
        void wait();
        void signal();
        void broadcast();

    private:
        PO6_NONCOPYABLE(cond);

    private:
        mutex* m_mtx;
        pthread_cond_t m_cond;
};

inline
cond :: cond(mutex* mtx)
            : m_mtx(mtx)
            , m_cond()
{
    int ret = pthread_cond_init(&m_cond, NULL);

    if (ret != 0)
    {
        abort();
    }
}

inline
cond :: ~cond() throw ()
{
    int ret = pthread_cond_destroy(&m_cond);

    if (ret != 0)
    {
        abort();
    }
}

inline void
cond :: lock()
{
    m_mtx->lock();
}

inline void
cond :: unlock()
{
    m_mtx->unlock();
}

inline void
cond :: wait()
{
    int ret = pthread_cond_wait(&m_cond, &m_mtx->m_mutex);

    if (ret != 0)
    {
        abort();
    }
}

inline void
cond :: signal()
{
    int ret = pthread_cond_signal(&m_cond);

    if (ret != 0)
    {
        abort();
    }
}

inline void
cond :: broadcast()
{
    int ret = pthread_cond_broadcast(&m_cond);

    if (ret != 0)
    {
        abort();
    }
}

} // namespace threads
} // namespace po6
#endif /* po6_threads_cond_h */
