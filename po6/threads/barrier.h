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

#ifndef po6_threads_barrier_h_
#define po6_threads_barrier_h_

// POSIX
#include <errno.h>
#include <pthread.h>

// po6
#include <po6/error.h>
#include <po6/noncopyable.h>
#include <po6/threads/cond.h>
#include <po6/threads/mutex.h>

namespace po6
{
namespace threads
{

class barrier
{
    public:
        barrier(size_t count);
        ~barrier() throw ();

    public:
        bool wait();

    private:
        PO6_NONCOPYABLE(barrier);

    private:
        po6::threads::mutex m_lock;
        po6::threads::cond m_cv;
        uint64_t m_height;
        uint64_t m_level;
        uint64_t m_generation;
};

inline
barrier :: barrier(size_t count)
    : m_lock()
    , m_cv(&m_lock)
    , m_height(count)
    , m_level(0)
    , m_generation(0)
{
}

inline
barrier :: ~barrier() throw ()
{
}

inline bool
barrier :: wait()
{
    int cancelstate;
    uint64_t gen;
    po6::threads::mutex::hold hold(&m_lock);
    gen = m_generation;
    ++m_level;

    if (m_level == m_height)
    {
        m_level = 0;
        ++m_generation;
        m_cv.broadcast();
        return true;
    }
    else
    {
        pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, &cancelstate);

        while (m_generation == gen)
        {
            m_cv.wait();
        }

        pthread_setcancelstate(cancelstate, NULL);
        return false;
    }
}

} // namespace threads
} // namespace po6

#endif // po6_threads_barrier_h_
