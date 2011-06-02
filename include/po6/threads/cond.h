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

#ifndef po6_threads_cond_h_
#define po6_threads_cond_h_

// POSIX
#include <errno.h>
#include <pthread.h>

// po6
#include "po6/error.h"
#include "po6/threads/mutex.h"

namespace po6
{
namespace threads
{

class cond
{
    public:
        cond(mutex* mtx)
            : m_mtx(mtx)
            , m_cond()
        {
            int ret = pthread_cond_init(&m_cond, NULL);

            if (ret != 0)
            {
                throw po6::error(ret);
            }
        }

        ~cond() throw ()
        {
            int ret = pthread_cond_destroy(&m_cond);

            if (ret != 0)
            {
                try
                {
                    PO6_DTOR_ERROR("Could not destroy condition variable.");
                }
                catch (...)
                {
                }
            }
        }

    public:
        void lock()
        {
            m_mtx->lock();
        }

        void trylock()
        {
            m_mtx->trylock();
        }

        void unlock()
        {
            m_mtx->unlock();
        }

        void wait()
        {
            int ret = pthread_cond_wait(&m_cond, &m_mtx->m_mutex);

            if (ret != 0)
            {
                throw po6::error(ret);
            }
        }

        void signal()
        {
            int ret = pthread_cond_signal(&m_cond);

            if (ret != 0)
            {
                throw po6::error(ret);
            }
        }

        void broadcast()
        {
            int ret = pthread_cond_broadcast(&m_cond);

            if (ret != 0)
            {
                throw po6::error(ret);
            }
        }

    private:
        cond(const cond&);

    private:
        cond& operator = (const cond&);

    private:
        mutex* m_mtx;
        pthread_cond_t m_cond;
};

} // namespace threads
} // namespace po6

#endif // po6_threads_cond_h_
